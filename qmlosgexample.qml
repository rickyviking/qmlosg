
import QtQuick 2.0
import qmlosg.globe 1.0

Rectangle {
    id: globeWin
    width: 1024
    height: 768



    GlobeOsg {
        id: globe1
        width: parent.width
        height: parent.height

        focus: true

        MouseArea {
            anchors {
                top: parent.top
                bottom: parent.bottom
                right: parent.right
                left: parent.left
            }

            onPressed: {
                globe1._mousePressEvent(mouse.x, mouse.y);
            }

            onPositionChanged: {
                globe1._mouseDragEvent(mouse.x, mouse.y);
            }
        }


    }


    Rectangle {
        id: zoomInButton
        color:"#8000FFFF"
        width: 150; height: 75

        Text{
            id: zoomInLabel
            anchors.centerIn: parent
            text: "zoom in"
        }

        MouseArea {
            anchors.fill: parent

            onClicked: {
                globe1.zoom = 0.9*globe1.zoom;

                if(zoomInButton.width  == 200)
                    zoomInButton.width = 150;
                else
                    zoomInButton.width = 200;
            }
        }
    }

    Rectangle {
        id: zoomOutButton
        color: "#80FFFF00"
        width: 150; height: 75
        anchors {
            top: parent.top
            right: parent.right
        }

        Text{
            id: zoomOutLabel
            anchors.centerIn: parent
            text: "zoom out"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: globe1.zoom = 1.1*globe1.zoom;
        }
    }


    Rectangle {
        id: homeButton
        color: "yellow"
        width: 150; height: 75
        anchors {
            bottom: parent.bottom
            left: parent.left
        }

        Text{
            id: homeLabel
            anchors.centerIn: parent
            text: "home"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: globe1.home();
        }
    }


    Rectangle {
        id: animateButton
        color: "green"
        width: 150; height: 75
        anchors {
            bottom: parent.bottom
            right: parent.right
        }

        Text{
            id: animateLabel
            anchors.centerIn: parent
            text: "animate"
        }


        MouseArea {
            anchors.fill: parent
            onClicked: {
                var currAnim = globe1.animate;
                if(currAnim)
                {
                    globe1.animate = false;
                    animateLabel = "animate";
                }
                else
                {
                    globe1.animate = true;
                    animateLabel = "stop animation";
                }
            }
        }
    }


}
