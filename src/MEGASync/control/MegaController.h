#pragma once

#include "megaapi.h"

#include <QString>
#include <QList>
//#include <memory>

class ProgressStep;
class ActionProgress;

/**
 * @brief The Controller class
 * TODO: complete docs
 *
 *
 */
class Controller
{
public:
    void addSync(const QString &localFolder, const mega::MegaHandle &remoteHandle, ActionProgress *progress = nullptr);
    static Controller *instance();

    //void doSth(mega::MegaApi *value); //example call

    void setApi(mega::MegaApi *value);

private:

    static Controller *model;

    mega::MegaApi *api;
    static Controller *controller;
};


/**
 * @brief A listener that receives an ActionProgress object to manage progress/completion/failure of a reuequest
 *
 *
 */
class ProgressFuncExecuterListener : public mega::MegaRequestListener
{
private:

    ActionProgress *mProgressHelper;

    std::function<void(mega::MegaApi* api, mega::MegaRequest *request, mega::MegaError *e)> onRequestFinishCallback;
    bool mAutoremove = true;
    bool executeInAppThread = true;

public:

    /**
     * @brief ProgressFuncExecuterListener
     * @param func to call upon onRequestFinish
     * @param autoremove whether this should be deleted after func is called
     */
    ProgressFuncExecuterListener(ActionProgress *progressHepler, bool autoremove = false,
                                 std::function<void(mega::MegaApi* api, mega::MegaRequest *request, mega::MegaError *e)> func = nullptr
                                 )
        : mProgressHelper(progressHepler), mAutoremove(autoremove), onRequestFinishCallback(std::move(func))
    {
    }

    void onRequestFinish(mega::MegaApi *api, mega::MegaRequest *request, mega::MegaError *e);
    virtual void onRequestStart(mega::MegaApi* api, mega::MegaRequest *request);
    virtual void onRequestUpdate(mega::MegaApi* api, mega::MegaRequest *request) {}
    virtual void onRequestTemporaryError(mega::MegaApi *api, mega::MegaRequest *request, mega::MegaError* e) {}
};


/**
 * @brief The ProgressHelper class
 * TODO:complete a little bit
 *
 * objects of this class will be deleted after emiting completion
 */
class ProgressHelper : public QObject
{
    Q_OBJECT

private:
    bool completed_signaled = false;
    double mPercentage = 0.0;
    QString mDescription;

    QList<ProgressStep> steps;
    int completedtasks = 0;

    bool mDeleteOnCompletion = false;

public:
    ProgressHelper(bool deleteOnCompletion = false, const QString description = QString());
    ~ProgressHelper();
    double percentage() const;
    void setPercentage(double value);
    void setComplete();
    ProgressHelper *addStep(const QString &description = QString(), double weight = 1.0);
    void addStep(ProgressHelper *task, double weight = 1.0);


    QString description() const;
    void setDescription(const QString &description);

    void checkCompletion();

private slots:
    void onStepProgress(double percentage, int position);
    void onStepCompleted(int position);

signals:
    void progress(double percentage);
    void completed();
};

/**
 * @brief Class to ensure that a progress helper created in a context gets completed if no subtasks are asigned to it
 */
class ProgressHelperCompletionGuard
{
private:
    ProgressHelper *mProgressHelper = nullptr;
public:
    ProgressHelperCompletionGuard(ProgressHelper *progressHelper) : mProgressHelper(progressHelper) {};
    ~ProgressHelperCompletionGuard()
    {
        if (mProgressHelper)
        {
            mProgressHelper->checkCompletion();
        }
    }
};

class ActionProgress : public ProgressHelper
{
    Q_OBJECT

private:
    QString mError;
public:
    ActionProgress(bool deleteOnCompletion = false, const QString description = QString());

    QString error() const;
    void setFailed(int errorCode); //note, this will call SetComplete() & hence emit a completion signal

signals:
    void failed(int errorCode);

};



class ProgressStep
{
public:
    ProgressStep(ProgressHelper *task, double weight = 1.0);
    ProgressHelper *task() const;
    double weight() const;


private:
    ProgressHelper *mTask;
    double mWeight;
};



