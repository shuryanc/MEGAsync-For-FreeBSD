// System
import QtQuick 2.15
import QtQuick.Controls 2.15

// C++
import ApiEnums 1.0
import LoginController 1.0

TwoFAPageForm {
    id: twoFAPageId

    onVisibleChanged: {
        console.log("TwoFAPageForm visible : " + visible)
    }

    Component.onCompleted: {

        console.log("TwoFAPageForm completed! : " + visible)

        twoFAField.forceFocus();

        /*
        console.log("1111111111111111111111111111111")
        if (visible) {
            console.log("twoFAField.forceFocus();")

        }
        */
    }

    signUpButton.onClicked: {
        loginControllerAccess.state = LoginController.SIGN_UP;
        loginControllerAccess.email = "";
    }

    loginButton.onClicked: {
        loginControllerAccess.login2FA(twoFAField.key);
    }

    twoFAField.onAllDigitsFilled: {
        loginControllerAccess.login2FA(twoFAField.key);
    }

    readonly property string validating2FA: "validating"
    readonly property string fetchingNodes2FA: "fetchingNodes2FA"
    readonly property string validating2FAFailed: "validating2FAFailed"
    readonly property string normal: "normal"

    function getState() {
        switch(loginControllerAccess.state) {
            case LoginController.LOGGING_IN_2FA_VALIDATING:
                return validating2FA;
            case LoginController.FETCHING_NODES_2FA:
                return fetchingNodes2FA;
            case LoginController.LOGGING_IN_2FA_FAILED:
                return validating2FAFailed;
            default:
                return normal;
        }
    }

    state: getState()
    states: [
        State {
            name: normal
            PropertyChanges {
                target: twoFAPageId
                enabled: true
            }
            PropertyChanges {
                target: loginButton
                icons.busyIndicatorVisible: false
            }
        },
        State {
            name: validating2FA
            PropertyChanges {
                target: twoFAPageId
                enabled: false
            }
            PropertyChanges {
                target: loginButton
                icons.busyIndicatorVisible: true
            }
        },
        State {
            name: validating2FAFailed
            extend: normal
            PropertyChanges {
                target: twoFAField
                hasError: true
            }
        },
        State {
            name: fetchingNodes2FA
            extend: validating2FA
        }
    ]
}
