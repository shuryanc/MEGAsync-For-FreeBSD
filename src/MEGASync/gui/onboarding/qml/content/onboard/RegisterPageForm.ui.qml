// System
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

// QML common
import Components.Buttons 1.0 as MegaButtons
import Components.Texts 1.0 as MegaTexts
import Components.Views 1.0 as MegaViews
import Common 1.0

// Local
import Onboard 1.0

// C++
import Onboarding 1.0

StackViewPage {
    id: root

    readonly property int contentHeight: 374

    property RegisterContent registerContent: RegisterContent {
        parent: scrollPanel.flickable.contentItem
    }

    property alias loginButton: loginButton
    property alias nextButton: nextButton

    Column {
        id: mainColumn

        anchors.left: root.left
        anchors.right: root.right
        anchors.top: root.top
        anchors.leftMargin: -3

        spacing: contentSpacing

        MegaTexts.RichText {
            id: title

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: registerContent.email.sizes.focusBorderWidth
            font.pixelSize: MegaTexts.Text.Size.Large
            text: OnboardingStrings.signUpTitle
        }

        MegaViews.ScrollPanel {
            id: scrollPanel

            anchors.left: parent.left
            anchors.right: parent.right
            height: root.contentHeight
            flickable.contentHeight: registerContent.implicitHeight
            flickable.contentWidth: registerContent.implicitWidth
        }
    }

    RowLayout {
        anchors {
            right: root.right
            bottom: root.bottom
            bottomMargin: 29
            left: root.left
        }

        MegaButtons.OutlineButton {
            id: loginButton

            Layout.alignment: Qt.AlignLeft
            Layout.leftMargin: -loginButton.sizes.focusBorderWidth
            text: OnboardingStrings.login
        }

        MegaButtons.PrimaryButton {
            id: nextButton

            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: -nextButton.sizes.focusBorderWidth
            enabled: registerContent.termsCheckBox.checked
            icons.source: Images.arrowRight
            text: OnboardingStrings.next
        }
    }
}
