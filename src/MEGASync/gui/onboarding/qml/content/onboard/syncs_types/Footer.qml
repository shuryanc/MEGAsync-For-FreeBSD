// System
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

// QML common
import Components.Buttons 1.0 as MegaButtons
import Common 1.0

// Local
import Onboard 1.0

// C++
import Onboarding 1.0

RowLayout {

    property alias notNowButton: notNowButton
    property alias previousButton: previousButton
    property alias nextButton: nextButton

    readonly property int horizontalMargin: 32
    readonly property int verticalMargin: 32

    width: parent.width

    MegaButtons.SecondaryButton {
        id: notNowButton

        Layout.leftMargin: horizontalMargin
        Layout.bottomMargin: verticalMargin
        text: OnboardingStrings.notNow
        onClicked: {
            onboardingWindow.close();
        }
    }

    RowLayout {
        Layout.alignment: Qt.AlignRight
        Layout.rightMargin: horizontalMargin
        Layout.bottomMargin: verticalMargin

        MegaButtons.OutlineButton {
            id: previousButton

            text: OnboardingStrings.previous
        }

        MegaButtons.PrimaryButton {
            id: nextButton

            text: OnboardingStrings.next
            icons.source: Images.arrowRight
        }

    }
}
