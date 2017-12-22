# IoT-QtClient

IoT-Ticket Qt client provides an easy-to-use library so that Your application can take advantage of the versatile IoT-Ticket cloud tools.

## System requirements

This Qt library can be used on all platforms Qt supports. Library uses Qt's Json libraries so minimum compliant Qt version is 5.0. 

## Getting started
1. Create your own IoT-Ticket account at https://www.iot-ticket.com/ (Request an invitation)
2. Wait for the account activation email
3. Build the library
4. Start using it in your application

### Building the library

Library uses a standard qmake project. Debug builds also build example application and unitests. Unitests support QtCreator's Auto Test plugin.
Compiler needs to support c++11 features.


### Example code

The library contains a demo which provides an example application. Html documentation in the doc folder contains also examples how library API's can be used.

### Providing authentication credentials for the IotTicket server connection

Credentials need to be set before other operations can be performed.
```cpp
iot::Connection* connection = iot::Connection::instance();
connection->setUserName("userName");
connection->setPassword("password");
// Connection is now configured and request can be made.
```

### Registering a device

```cpp
    iot::Device* device = new iot::Device();
    device->setManufacturer("Device manufacturer");
    device->setDescription("Device description");
    device->setName("Device name");
    device->setType("Device type");
    QVariantMap attributes;
    attributes["id"] = 123;
    device->setAttributes(attributes);
    connect(device, &iot::Device::registerFinished, [=](bool success) {
        if (success) {
            qDebug() << "done";
            QString deviceId = device->deviceId();
            // device id needed in future operations
            device->deleteLater();
        } else {
            qDebug() << "Error";
        }
    } );

    device->registerDevice();
```

### Listing devices
```cpp
    iot::DeviceList* deviceList = new iot::DeviceList();
    connect(deviceList, &iot::DeviceList::getFinished, [=](bool success) {
        if (success) {
            qDebug() << "done";
            QList<iot::Device*> devices = deviceList->devices();
            // Do something with the devices.
            deviceList->deleteLater();
        } else {
            qDebug() << "error";
        }
    } );

    deviceList->get();
```

### Get device datanodes
```cpp
    iot::Device* device = new iot::Device();
    device->setDeviceId("c5c7fdba5c1c48c1b03372c4ce512e48");
    connect(device, &iot::Device::getDataNodesFinished, [=](bool success) {
        if (success) {
            qDebug() << "done";
            QList<iot::DataNode*> dataNodes = device->dataNodes();
            // Do something with the datanodes.
            device->deleteLater();
        } else {
            qDebug() << "error";
        }
    } );

    device->getDataNodes();

```

### Write data
Library contains several convenience functions for writing data. Here is one way.
```cpp
    iot::DataNode* node = new iot::DataNode();
    node->setDeviceId("c5c7fdba5c1c48c1b03372c4ce512e48");
    node->setPath("engine");
    node->setName("fan");
    node->setUnit("RPM");
    node->setDataType(iot::DataNode::Long); // Not mandatory
    connect(node, &iot::DataNode::writeFinished, [=](bool success) {
        if (success) {
            qDebug() << "done";
            node->deleteLater();
        } else {
            qDebug() << "error";
        }
    } );

    node->writeValue(2000);
```

### Read data
Library contains several convenience functions for reading data. Here is one way.
```cpp
    iot::DataNode* node = new iot::DataNode();
    node->setDeviceId("c5c7fdba5c1c48c1b03372c4ce512e48");
    node->setPath("engine");
    node->setName("fan");
    connect(node, &iot::DataNode::readFinished, [=](bool success) {
        if (success) {
            qDebug() << "success";
            QPair<QVariant, QDateTime> val = node->latestValue();
            node->deleteLater();
        } else {
            qDebug() << "Error";
        }
    });

    node->readLatestValue();
```

## API documentation
This Qt client library uses the IoT-Ticket REST API. The documentation for the underlying REST service can be found from
https://www.iot-ticket.com/images/Files/IoT-Ticket.com_IoT_API.pdf. 

doc folder contains detailed html API codumentation that is generated from the sources codes.
