// System
import QtQuick 2.12
import QtQuick.Layouts 1.12

// Local
import Onboard 1.0
import Onboard.Syncs_types 1.0

SyncsPage {

    ColumnLayout {

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        spacing: 24

        Header {
            title: OnboardingStrings.selectBackupFoldersTitle
            description: OnboardingStrings.selectBackupFoldersDescription
        }

        ColumnLayout {

            Layout.preferredWidth: parent.width
            spacing: 12

            InfoAccount {
                Layout.preferredWidth: parent.width
            }

            SelectTable {}
        }
    }
}
