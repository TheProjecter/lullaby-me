# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

symbian:TARGET.UID3 = 0x20041FB2

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices PowerMgmt

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
# CONFIG += mobility
# MOBILITY +=

SOURCES += main.cpp mainwindow.cpp
HEADERS += mainwindow.h
FORMS += mainwindow.ui

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

symbian {
    ICON = icon.svg
    TARGET.UID3 = 0x20041FB2
    TARGET.CAPABILITY = PowerMgmt
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
    LIBS += -lcone -leikcore -lavkon

    vendorinfo = \
        "%{\"De La Salle University Manila\"}" \
        ":\"De La Salle University Manila\""

    my_deployment.pkg_prerules = vendorinfo

    # Remove all the existing platform dependencies
    default_deployment.pkg_prerules -= pkg_platform_dependencies
    # Add a dependency for just the Symbian^1 and Symbian^3 and later phones
    supported_platforms = \
        "; Application that only supports Symbian^1 and Symbian^3" \
        "[0x1028315F],0,0,0,{\"S60ProductID\"}" \
        "[0x20022E6D],0,0,0,{\"S60ProductID\"}"


    my_deployment.pkg_prerules += supported_platforms

    DEPLOYMENT += my_deployment

    DEPLOYMENT.installer_header= 0x2002CCCF
}

RESOURCES += \
    resources.qrc
