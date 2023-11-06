// System
import QtQuick 2.15
import QtQuick.Layouts 1.15

// QML common
import Common 1.0
import Components.Texts 1.0 as MegaTexts
import Components.Images 1.0 as MegaImages
import Components.BusyIndicator 1.0 as MegaBusyIndicator

// Local
import Onboard 1.0

// C++
import BackupsProxyModel 1.0
import BackupsModel 1.0

Rectangle {
    id: tableRectangle

    readonly property int headerFooterMargin: 24
    readonly property int headerFooterHeight: 40
    readonly property int tableRadius: 8

    Layout.preferredWidth: width
    Layout.preferredHeight: height
    height: backupsModelAccess.globalError === backupsModelAccess.BackupErrorCode.NONE
            ? 192 : 192 - headerFooterHeight + listView.footerItem.height
    width: parent.width
    radius: tableRadius
    color: Styles.pageBackground

    Rectangle {
        id: borderRectangle

        width: tableRectangle.width
        height: tableRectangle.height
        color: "transparent"
        border.color: Styles.borderStrong
        border.width: 1
        radius: 8
        z: 5
    }

    ListView {
        id: listView

        model: backupsProxyModel
        anchors.fill: parent
        headerPositioning: ListView.OverlayHeader
        focus: true
        clip: true
        delegate: folderComponent
        header: headerComponent
        footerPositioning: ListView.OverlayFooter
        footer: footerComponent
    }

    Component {
        id: headerComponent

        Rectangle {
            height: headerFooterHeight
            anchors.left: parent.left
            anchors.right: parent.right
            color: Styles.pageBackground
            radius: tableRectangle.radius
            z: 3

            RowLayout {
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
                spacing: 0

                RowLayout {
                    Layout.leftMargin: headerFooterMargin
                    Layout.fillWidth: true
                    spacing: headerFooterMargin / 2

                    MegaImages.SvgImage {
                        source: Images.database
                        color: Styles.iconPrimary
                        sourceSize: Qt.size(16, 16)
                    }

                    MegaTexts.Text {
                        text: OnboardingStrings.backupFolders
                        font.weight: Font.DemiBold
                        color: Styles.textPrimary
                    }
                }

                MegaTexts.Text {
                    id: totalSizeText

                    Layout.rightMargin: headerFooterMargin
                    Layout.alignment: Qt.AlignRight
                    text: backupsModelAccess.totalSize
                    font.pixelSize: MegaTexts.Text.Size.Small
                    font.weight: Font.DemiBold
                    color: Styles.textPrimary
                    visible: backupsModelAccess.totalSizeReady
                }

                MegaBusyIndicator.BusyIndicator {
                    Layout.rightMargin: headerFooterMargin
                    Layout.alignment: Qt.AlignRight
                    visible: !backupsModelAccess.totalSizeReady
                    color: Styles.textAccent
                    imageSize: Qt.size(16, 16)
                    Layout.preferredWidth: 16
                    Layout.preferredHeight: 16
                }
            }

            Rectangle {
                height: borderRectangle.border.width
                color: Styles.borderSubtle
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
            }
        }
    }

    Component {
        id: folderComponent

        FolderRow {
            id: folderRow

            anchors.right: parent.right
            anchors.left: parent.left
        }
    }

    Component {
        id: footerComponent

        Rectangle {
            id: notificationFooter

            anchors.left: parent.left
            anchors.right: parent.right
            height: notificationItem.height
            radius: tableRadius
            color: Styles.pageBackground
            z: 3
            visible: backupsModelAccess.globalError !== backupsModelAccess.BackupErrorCode.NONE

            MegaTexts.NotificationText {
                id: notificationItem

                width: parent.width
                type: backupsModelAccess.globalError === backupsModelAccess.BackupErrorCode.SDK_CREATION
                        ? Constants.MessageType.ERROR
                        : Constants.MessageType.WARNING
                topBorderRect: true
                text: backupsModelAccess.conflictsNotificationText
                visible: parent.visible
            }
        }
    }
}
