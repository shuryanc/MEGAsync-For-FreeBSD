#include "MegaItem.h"
#include "QMegaMessageBox.h"
#include "MegaApplication.h"
#include "AvatarWidget.h"
#include "model/SyncModel.h"
#include "MegaApplication.h"
#include "mega/utils.h"
#include "UserAttributesRequests.h"

#include <QByteArray>


const int MegaItem::ICON_SIZE = 17;

using namespace mega;

MegaItem::MegaItem(std::unique_ptr<MegaNode> node, MegaItem *parentItem, bool showFiles) :
    QObject(parentItem),
    mShowFiles(showFiles),
    mOwnerEmail(QString()),
    mStatus(STATUS::NONE),
    mCameraFolder(false),
    mChatFilesFolder(false),
    mChildrenSet(false),
    mNode(std::move(node)),
    mOwner(nullptr),
    mMegaApi(MegaSyncApp->getMegaApi())
{ 
    if(isRoot() || mNode->isFile() || mNode->isInShare())
    {
        mStatus = STATUS::NONE;
        return;
    }

    //This code is to calculate if a folder which is inside an incoming share folder is a child of a sync
    //////////
    MegaItem *parent_item = getParent();
    while(parent_item && parent_item->getNode()->getParentHandle() != INVALID_HANDLE)
    {
        parent_item = parent_item->getParent();
    }

    QStringList folderList;
    if(isVault() || (parent_item && parent_item->isVault()))
    {
        mStatus = STATUS::BACKUP;
        return;
    }
    if(parent_item && parent_item->getNode()->isInShare())
    {
        foreach(const QString& folder, SyncModel::instance()->getMegaFolders(SyncModel::AllHandledSyncTypes))
        {
            if(folder.startsWith(parent_item->getOwnerEmail()))
            {
                folderList.append(folder.split(QLatin1Char(':')).last().prepend(QLatin1Char('/')));
            }
        }
        calculateSyncStatus(folderList);
    }
    ////////////
    else
    {
        calculateSyncStatus(SyncModel::instance()->getMegaFolders(SyncModel::AllHandledSyncTypes));
    }
}


std::shared_ptr<mega::MegaNode> MegaItem::getNode()
{
    return mNode;
}

void MegaItem::setChildren(std::shared_ptr<MegaNodeList> children)
{
    mChildrenSet = true;
    for (int i = 0; i < children->size(); i++)
    {
        auto node = std::unique_ptr<MegaNode>(children->get(i)->copy());
        if (!mShowFiles && node->getType() == MegaNode::TYPE_FILE)
        {
            break;
        }
        mChildItems.append(new MegaItem(move(node), this, mShowFiles));
    }
}

bool MegaItem::areChildrenSet()
{
    return mChildrenSet;
}

MegaItem *MegaItem::getParent()
{
    return dynamic_cast<MegaItem*>(parent());
}

MegaItem* MegaItem::getChild(int i)
{
    return mChildItems.at(i);
}

int MegaItem::getNumChildren()
{
    return mChildItems.size();
}

int MegaItem::indexOf(MegaItem* item)
{
    return mChildItems.indexOf(item);
}

QString MegaItem::getOwnerName()
{
    if(mFullNameAttribute && mFullNameAttribute->isAttributeReady())
    {
        return mFullNameAttribute->getFullName();
    }

    return mOwnerEmail;
}

QString MegaItem::getOwnerEmail()
{
    return mOwnerEmail;
}

void MegaItem::setOwner(std::unique_ptr<mega::MegaUser> user)
{
    mOwner = std::move(user);
    mOwnerEmail = QString::fromUtf8(mOwner->getEmail());
    mFullNameAttribute = UserAttributes::FullNameAttributeRequest::requestFullName(mOwner->getEmail());
    if(mFullNameAttribute)
    {
        connect(mFullNameAttribute.get(), &UserAttributes::FullNameAttributeRequest::attributeReady, this, &MegaItem::onFullNameAttributeReady);
        if(mFullNameAttribute->isAttributeReady())
        {
            onFullNameAttributeReady();
        }
    }
    mAvatarAttribute = UserAttributes::AvatarAttributeRequest::requestAvatar(mOwner->getEmail());
    if(mAvatarAttribute)
    {
        connect(mAvatarAttribute.get(), &UserAttributes::AvatarAttributeRequest::attributeReady, this, &MegaItem::onAvatarAttributeReady);
        if(mAvatarAttribute->isAttributeReady())
        {
            onAvatarAttributeReady();
        }
    }

    QStringList folderList;
    //Calculating if we have a synced childs.
    foreach(const QString& folder, SyncModel::instance()->getMegaFolders(SyncModel::AllHandledSyncTypes))
    {
        if(folder.startsWith(mOwnerEmail))
        {
            folderList.append(folder.split(QLatin1Char(':')).last().prepend(QLatin1Char('/')));
        }
    }
    calculateSyncStatus(folderList);
}

void MegaItem::onFullNameAttributeReady()
{
    emit infoUpdated(Qt::DisplayRole);
}

void MegaItem::onAvatarAttributeReady()
{
    emit infoUpdated(Qt::DecorationRole);
}

QPixmap MegaItem::getOwnerIcon()
{
    if(mAvatarAttribute)
    {
        return mAvatarAttribute->getPixmap(ICON_SIZE);
    }

    return QPixmap();
}

QIcon MegaItem::getStatusIcons()
{
    QIcon statusIcons; //first is selected state icon / second is normal state icon
    switch(mStatus)
    {
    case STATUS::SYNC:
    {
        statusIcons.addFile(QLatin1String("://images/Item-sync-press.png"), QSize(), QIcon::Selected); //normal style icon
        statusIcons.addFile(QLatin1String("://images/Item-sync-rest.png"), QSize(), QIcon::Normal); //normal style icon
        break;
    }
    case STATUS::SYNC_PARENT:
    {
        statusIcons.addFile(QLatin1String("://images/Item-sync-press.png"), QSize(), QIcon::Selected); //normal style icon
        statusIcons.addFile(QLatin1String("://images/node_selector/icon-small-sync-disabled.png"), QSize(), QIcon::Normal); //normal style icon
        break;
    }
    default:
    {
        break;
    }
    }

    return statusIcons;
}

QIcon MegaItem::getFolderIcon()
{
    if (getNode()->getType() >= MegaNode::TYPE_FOLDER)
    {
        if(mCameraFolder)
        {
            QIcon icon;
            icon.addFile(QLatin1String("://images/node_selector/small-camera-sync.png"), QSize(), QIcon::Normal);
            icon.addFile(QLatin1String("://images/node_selector/small-folder-camera-sync-disabled.png"), QSize(), QIcon::Disabled);
            return icon;
        }
        else if(mChatFilesFolder)
        {
            QIcon icon;
            icon.addFile(QLatin1String("://images/node_selector/small-chat-files.png"), QSize(), QIcon::Normal);
            icon.addFile(QLatin1String("://images/node_selector/small-chat-files-disabled.png"), QSize(), QIcon::Disabled);
            return icon;
        }
        else if (getNode()->isInShare())
        {
            QIcon icon;
            icon.addFile(QLatin1String("://images/node_selector/small-folder-incoming.png"), QSize(), QIcon::Normal);
            icon.addFile(QLatin1String("://images/node_selector/small-folder-incoming-disabled.png"), QSize(), QIcon::Disabled);
            return icon;
        }
        else if (getNode()->isOutShare())
        {
            QIcon icon;
            icon.addFile(QLatin1String("://images/node_selector/small-folder-outgoing.png"), QSize(), QIcon::Normal);
            icon.addFile(QLatin1String("://images/node_selector/small-folder-outgoing_disabled.png"), QSize(), QIcon::Disabled);
            return icon;
        }
        else if(isRoot())
        {
            QIcon icon;
            icon.addFile(QLatin1String("://images/node_selector/cloud.png"));
            return icon;
        }
        else if(isVault())
        {
            QIcon icon;
            icon.addFile(QLatin1String("://images/node_selector/Backups_small_ico.png"));
            return icon;
        }
        else
        {
            QString nodeDeviceId (QString::fromUtf8(getNode()->getDeviceId()));
            if (!nodeDeviceId.isEmpty())
            {
                std::unique_ptr<mega::MegaNode> parent (mMegaApi->getNodeByHandle(getNode()->getParentHandle()));
                if (parent)
                {
                    QString parentDeviceId (QString::fromUtf8(parent->getDeviceId()));
                    if (parentDeviceId.isEmpty())
                    {
                        // TODO, future: choose icon according to host OS
                        if (nodeDeviceId == QString::fromUtf8(mMegaApi->getDeviceId()))
                        {
                            #ifdef Q_OS_WINDOWS
                            static const QIcon thisDeviceIcon (QLatin1String("://images/icons/pc/pc-win_24.png"));
                            #elif defined(Q_OS_MACOS)
                            static const QIcon thisDeviceIcon (QLatin1String("://images/icons/pc/pc-mac_24.png"));
                            #elif defined(Q_OS_LINUX)
                            static const QIcon thisDeviceIcon (QLatin1String("://images/icons/pc/pc-linux_24.png"));
                            #endif
                            return thisDeviceIcon;
                        }
                        else
                        {
                            static const QIcon genericIcon (QLatin1String("://images/icons/pc/pc_24.png"));
                            return genericIcon;
                        }
                    }
                }
            }

            QIcon icon;
            icon.addFile(QLatin1String("://images/small_folder.png"), QSize(), QIcon::Normal);
            icon.addFile(QLatin1String("://images/node_selector/small-folder-disabled.png"), QSize(), QIcon::Disabled);
            return icon;
        }
    }
    else
    {
        return Utilities::getExtensionPixmapSmall(QString::fromUtf8(getNode()->getName()));
    }
}

int MegaItem::getStatus()
{
    return mStatus;
}

bool MegaItem::isSyncable()
{       
    return mStatus != SYNC
            && mStatus != SYNC_PARENT
            && mStatus != SYNC_CHILD
            && mStatus != BACKUP;
}

int MegaItem::insertPosition(const std::unique_ptr<MegaNode>& node)
{
    int type = node->getType();

    int i;
    for (i = 0; i < mChildItems.size(); i++)
    {
        std::shared_ptr<MegaNode> n = mChildItems.at(i)->getNode();
        int nodeType = n->getType();
        if (type < nodeType)
        {
            continue;
        }

        if (qstricmp(node->getName(), n->getName()) <= 0)
        {
            break;
        }
    }

    return i;
}

void MegaItem::insertNode(std::unique_ptr<MegaNode>node, int index)
{
    mChildItems.insert(index, new MegaItem(move(node), this, mShowFiles));
}

void MegaItem::removeNode(std::shared_ptr<MegaNode> node)
{
    if (!node)
    {
        return;
    }

    for (int i = 0; i < mChildItems.size(); i++)
    {
        if (mChildItems[i]->getNode()->getHandle() == node->getHandle())
        {
            MegaItem* item = mChildItems.takeAt(i);
            delete item;
            return;
        }
    }
}

void MegaItem::displayFiles(bool enable)
{
    mShowFiles = enable;
}

void MegaItem::setCameraFolder()
{
    mCameraFolder = true;
}

void MegaItem::setChatFilesFolder()
{
    mChatFilesFolder = true;
}

int MegaItem::row()
{
    if (MegaItem* parent = getParent())
    {
        return parent->mChildItems.indexOf(const_cast<MegaItem*>(this));
    }
    return 0;
}

MegaItem::~MegaItem()
{
    qDeleteAll(mChildItems);
    mChildItems.clear();
}

void MegaItem::calculateSyncStatus(const QStringList &folders)
{
    auto syncedFolders = SyncModel::instance()->getMegaFolderHandles(SyncModel::AllHandledSyncTypes);
    if(syncedFolders.contains(mNode->getHandle()))
    {
        mStatus = STATUS::SYNC;
        return;
    }

    QString parentFolders;
    std::shared_ptr<MegaNode> n = mNode;
    parentFolders.append(QLatin1Char('/'));
    parentFolders.append(QString::fromUtf8(n->getName()));
    while(n->getParentHandle () != INVALID_HANDLE)
    {
        n = std::shared_ptr<MegaNode>(mMegaApi->getNodeByHandle(n->getParentHandle()));
        if(n->getType() != MegaNode::TYPE_ROOT)
        {
            parentFolders.prepend(QString::fromUtf8(n->getName()));
            parentFolders.prepend(QLatin1Char('/'));
        }
    }

    foreach(const QString& syncFolder, folders)
    {
        if(syncFolder.startsWith(parentFolders))
        {
            mStatus = STATUS::SYNC_PARENT;
        }
        else if(parentFolders.startsWith(syncFolder))
        {
            mStatus = STATUS::SYNC_CHILD;
        }
    }
}

bool MegaItem::isRoot()
{
    return mNode->getHandle() == MegaSyncApp->getRootNode()->getHandle();
}

bool MegaItem::isVault()
{
    if(auto restNode = MegaSyncApp->getVaultNode())
        return getNode()->getHandle() == restNode->getHandle();

    return false;
}
