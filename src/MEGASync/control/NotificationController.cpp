#include "NotificationController.h"

#include "MegaApplication.h"

NotificationController::NotificationController(QObject* parent)
    : QObject(parent)
    , mMegaApi(MegaSyncApp->getMegaApi())
    , mGlobalListener(std::make_unique<mega::QTMegaGlobalListener>(MegaSyncApp->getMegaApi(), this))
    , mNotificationAlertModel(nullptr)
    , mNotificationAlertDelegate(nullptr)
    , mAlertsProxyModel(nullptr)
{
    mMegaApi->addGlobalListener(mGlobalListener.get());
}

void NotificationController::populateUserAlerts(mega::MegaUserAlertList* alertList, bool copyRequired)
{
    if (!alertList)
    {
        return;
    }

    emit userAlertsUpdated(alertList);

    if (mNotificationAlertModel && mNotificationAlertModel->alertsModel())
    {
        mNotificationAlertModel->alertsModel()->insertAlerts(alertList, copyRequired);
    }
    else
    {
        QAlertsModel* alertsModel = new QAlertsModel(alertList, copyRequired);
        mAlertsProxyModel = new QFilterAlertsModel();
        mAlertsProxyModel->setSourceModel(alertsModel);
        mAlertsProxyModel->setSortRole(Qt::UserRole); //Role used to sort the model by date.
        MegaAlertDelegate* alertDelegate = new MegaAlertDelegate(alertsModel, false, this);

        QNotificationsModel* notificationsModel = new QNotificationsModel();
        MegaNotificationDelegate* notificationDelegate = new MegaNotificationDelegate(notificationsModel, this);

        mNotificationAlertModel = new NotificationAlertModel(notificationsModel, alertsModel);
        mNotificationAlertDelegate = new NotificationAlertDelegate(notificationDelegate, alertDelegate);

        emit notificationAlertCreated(mNotificationAlertModel, mNotificationAlertDelegate);
    }

    emit unseenAlertsChanged(mNotificationAlertModel->alertsModel()->getUnseenNotifications());

    if (!copyRequired)
    {
        alertList->clear(); //empty the list otherwise they will be deleted
        delete alertList;
    }
}

void NotificationController::onUserAlertsUpdate(mega::MegaApi* api, mega::MegaUserAlertList* list)
{
    Q_UNUSED(api)

    if (MegaSyncApp->finished())
    {
        return;
    }

    // if we have a list, we don't need to query megaApi for it and block the sdk mutex, we do this
    // synchronously, since we are not copying the list, and we need to process it before it goes out of scope.
    bool doSynchronously{list != NULL};

    if (doSynchronously)
    {
        populateUserAlerts(list, true);
    }
    else
    {
        auto funcToThreadPool = [this]()
        { //thread pool function
            mega::MegaUserAlertList* theList;
            theList = mMegaApi->getUserAlerts();
            //queued function
            Utilities::queueFunctionInAppThread([this, theList]()
            {
                populateUserAlerts(theList, false);
            });
        }; // end of thread pool function
        ThreadPoolSingleton::getInstance()->push(funcToThreadPool);
    }
}

bool NotificationController::alertsAreFiltered()
{
    return mAlertsProxyModel
            && mAlertsProxyModel->filterAlertType() != QFilterAlertsModel::NO_FILTER;
}

bool NotificationController::hasAlerts()
{
    return mNotificationAlertModel->alertsModel()
            && mNotificationAlertModel->alertsModel()->rowCount(QModelIndex());
}

bool NotificationController::hasAlertsOfType(int type)
{
    return mNotificationAlertModel->alertsModel()
            && mNotificationAlertModel->alertsModel()->existsNotifications(type);
}

void NotificationController::applyNotificationFilter(int opt)
{
    if (mAlertsProxyModel)
    {
        mAlertsProxyModel->setFilterAlertType(opt);
    }
}
