#include "InfoDialogTransfersProxyModel.h"

#include "InfoDialogTransferDelegateWidget.h"
#include "TransfersModel.h"

//SORT FILTER PROXY MODEL
InfoDialogTransfersProxyModel::InfoDialogTransfersProxyModel(QObject *parent) : TransfersSortFilterProxyBaseModel(parent)
{
}

InfoDialogTransfersProxyModel::~InfoDialogTransfersProxyModel()
{
}

TransferBaseDelegateWidget* InfoDialogTransfersProxyModel::createTransferManagerItem(QWidget *parent)
{
    auto item = new InfoDialogTransferDelegateWidget(parent);

    connect(item, &InfoDialogTransferDelegateWidget::copyTransferLink,
            this, &InfoDialogTransfersProxyModel::onCopyTransferLinkRequested);
    connect(item, &InfoDialogTransferDelegateWidget::openTransferFolder,
            this, &InfoDialogTransfersProxyModel::onOpenTransferFolderRequested);
    connect(item, &InfoDialogTransferDelegateWidget::retryTransfer,
            this, &InfoDialogTransfersProxyModel::onRetryTransferRequested);

    return item;
}

void InfoDialogTransfersProxyModel::onCopyTransferLinkRequested()
{
    auto delegateWidget = dynamic_cast<InfoDialogTransferDelegateWidget*>(sender());
    auto sourModel = dynamic_cast<TransfersModel*>(sourceModel());

    if(delegateWidget && sourModel)
    {
        QList<int> rows;
        auto index = delegateWidget->getCurrentIndex();
        index = mapToSource(index);
        rows.append(index.row());
        sourModel->getLinks(rows);
    }
}

void InfoDialogTransfersProxyModel::onOpenTransferFolderRequested()
{
    auto delegateWidget = dynamic_cast<InfoDialogTransferDelegateWidget*>(sender());
    auto sourModel = dynamic_cast<TransfersModel*>(sourceModel());

    if(delegateWidget && sourModel)
    {
        auto index = delegateWidget->getCurrentIndex();
        index = mapToSource(index);
        sourModel->openFolderByIndex(index);
    }
}

void InfoDialogTransfersProxyModel::onRetryTransferRequested()
{
    auto delegateWidget = dynamic_cast<InfoDialogTransferDelegateWidget*>(sender());
    auto sourModel = dynamic_cast<TransfersModel*>(sourceModel());

    if(delegateWidget && sourModel)
    {
        sourModel->retryTransferByIndex(mapToSource(delegateWidget->getCurrentIndex()));
    }
}

bool InfoDialogTransfersProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    bool lessThan (false);
    const auto leftItem (qvariant_cast<TransferItem>(left.data()).getTransferData());
    const auto rightItem (qvariant_cast<TransferItem>(right.data()).getTransferData());

    int leftItemState = leftItem->mState;
    int rightItemState = rightItem->mState;

    if(leftItemState != rightItemState)
    {
       lessThan = leftItemState < rightItemState;
    }
    else
    {
       lessThan = leftItem->mPriority < rightItem->mPriority;
    }

    return lessThan;
}

bool InfoDialogTransfersProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    bool accept (false);

    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    if(index.isValid())
    {
       const auto d (qvariant_cast<TransferItem>(index.data()).getTransferData());

       accept = (d->mState & TransferData::TransferState::TRANSFER_COMPLETED
                 || d->mState & TransferData::TransferState::TRANSFER_COMPLETING
                 || d->mState & TransferData::TransferState::TRANSFER_ACTIVE || d->mState & TransferData::TransferState::TRANSFER_FAILED);
    }

    return accept;
}
