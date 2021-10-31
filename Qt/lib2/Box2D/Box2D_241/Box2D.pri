
DEPENDPATH += \
    $$PWD/Box2D \
    $$PWD/Box2D/Collision \
    $$PWD/Box2D/Common \
    $$PWD/Box2D/Dynamics \
    $$PWD/Box2D/Rope \
    $$PWD/Box2D/Collision/Shapes \
    $$PWD/Box2D/Dynamics/Contacts \
    $$PWD/Box2D/Dynamics/Joints
INCLUDEPATH += $$DEPENDPATH

OTHER_FILES += version.txt

HEADERS += \
    b2Api.h \
    b2Types.h \
    b2Common.h \
    b2Shape.h \
    b2Joint.h \
    b2TimeStep.h \
    b2WorldCallbacks.h \
    b2GrowableStack.h \
    b2WheelJoint.h \
    b2GearJoint.h \
    b2RevoluteJoint.h \
    b2PrismaticJoint.h \
    b2DistanceJoint.h \
    b2MouseJoint.h \
    b2FrictionJoint.h \
    b2WeldJoint.h \
    b2PulleyJoint.h \
    b2MotorJoint.h \
    b2Fixture.h \
    b2Joint.h \
    b2Body.h \
    b2Island.h \
    b2World.h \
    b2CircleContact.h \
    b2ContactManager.h \
    b2EdgeCircleContact.h \
    b2ContactSolver.h \
    b2PolygonContact.h \
    b2Contact.h \
    b2ChainPolygonContact.h \
    b2PolygonCircleContact.h \
    b2EdgePolygonContact.h \
    b2ChainCircleContact.h \
    b2BlockAllocator.h \
    b2StackAllocator.h \
    b2Timer.h \
    b2Draw.h \
    b2Math.h \
    b2Settings.h \
    b2Rope.h \
    b2Distance.h \
    b2DynamicTree.h \
    b2EdgeShape.h \
    b2CircleShape.h \
    b2ChainShape.h \
    b2PolygonShape.h \
    b2Collision.h \
    b2TimeOfImpact.h \
    b2BroadPhase.h \

SOURCES += \
    b2RevoluteJoint.cpp \
    b2FrictionJoint.cpp \
    b2PulleyJoint.cpp \
    b2GearJoint.cpp \
    b2WeldJoint.cpp \
    b2PrismaticJoint.cpp \
    b2DistanceJoint.cpp \
    b2MouseJoint.cpp \
    b2MotorJoint.cpp \
    b2WheelJoint.cpp \
    b2Body.cpp \
    b2Joint.cpp \
    b2WorldCallbacks.cpp \
    b2World.cpp \
    b2Island.cpp \
    b2Fixture.cpp \
    b2PolygonCircleContact.cpp \
    b2EdgeCircleContact.cpp \
    b2CircleContact.cpp \
    b2ChainCircleContact.cpp \
    b2Contact.cpp \
    b2EdgePolygonContact.cpp \
    b2PolygonContact.cpp \
    b2ChainPolygonContact.cpp \
    b2ContactSolver.cpp \
    b2ContactManager.cpp \
    b2Settings.cpp \
    b2Math.cpp \
    b2StackAllocator.cpp \
    b2Timer.cpp \
    b2Draw.cpp \
    b2BlockAllocator.cpp \
    b2Rope.cpp \
    b2ChainShape.cpp \
    b2CircleShape.cpp \
    b2EdgeShape.cpp \
    b2PolygonShape.cpp \
    b2CollideEdge.cpp \
    b2TimeOfImpact.cpp \
    b2Collision.cpp \
    b2DynamicTree.cpp \
    b2CollideCircle.cpp \
    b2CollidePolygon.cpp \
    b2Distance.cpp \
    b2BroadPhase.cpp
