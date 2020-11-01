
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
    b2BroadPhase.h \
    b2Collision.h \
    b2Distance.h \
    b2DynamicTree.h \
    b2TimeOfImpact.h \
    b2BlockAllocator.h \
    b2Draw.h \
    b2GrowableStack.h \
    b2Math.h \
    b2Settings.h \
    b2StackAllocator.h \
    b2Timer.h \
    b2Body.h \
    b2ContactManager.h \
    b2Fixture.h \
    b2Island.h \
    b2TimeStep.h \
    b2World.h \
    b2WorldCallbacks.h \
    Rope/b2Rope.h \
    Shapes/b2ChainShape.h \
    b2CircleShape.h \
    b2EdgeShape.h \
    b2PolygonShape.h \
    b2Shape.h \
    b2ChainAndCircleContact.h \
    b2ChainAndPolygonContact.h \
    b2CircleContact.h \
    b2Contact.h \
    b2ContactSolver.h \
    b2EdgeAndCircleContact.h \
    b2EdgeAndPolygonContact.h \
    b2PolygonAndCircleContact.h \
    b2PolygonContact.h \
    b2DistanceJoint.h \
    b2FrictionJoint.h \
    b2GearJoint.h \
    b2Joint.h \
    b2MouseJoint.h \
    b2PrismaticJoint.h \
    b2PulleyJoint.h \
    b2RevoluteJoint.h \
    b2RopeJoint.h \
    b2WeldJoint.h \
    b2WheelJoint.h \
    b2MotorJoint.h

SOURCES += \
    b2BroadPhase.cpp \
    b2CollideCircle.cpp \
    b2CollideEdge.cpp \
    b2CollidePolygon.cpp \
    b2Collision.cpp \
    b2Distance.cpp \
    b2DynamicTree.cpp \
    b2TimeOfImpact.cpp \
    b2BlockAllocator.cpp \
    b2Draw.cpp \
    b2Math.cpp \
    b2Settings.cpp \
    b2StackAllocator.cpp \
    b2Timer.cpp \
    b2Body.cpp \
    b2ContactManager.cpp \
    b2Fixture.cpp \
    b2Island.cpp \
    b2World.cpp \
    b2WorldCallbacks.cpp \
    Rope/b2Rope.cpp \
    b2ChainShape.cpp \
    b2CircleShape.cpp \
    b2EdgeShape.cpp \
    b2PolygonShape.cpp \
    b2ChainAndCircleContact.cpp \
    b2ChainAndPolygonContact.cpp \
    b2CircleContact.cpp \
    b2Contact.cpp \
    b2ContactSolver.cpp \
    b2EdgeAndCircleContact.cpp \
    b2EdgeAndPolygonContact.cpp \
    b2PolygonAndCircleContact.cpp \
    b2PolygonContact.cpp \
    b2DistanceJoint.cpp \
    b2FrictionJoint.cpp \
    b2GearJoint.cpp \
    b2Joint.cpp \
    b2MouseJoint.cpp \
    b2PrismaticJoint.cpp \
    b2PulleyJoint.cpp \
    b2RevoluteJoint.cpp \
    b2RopeJoint.cpp \
    b2WeldJoint.cpp \
    b2WheelJoint.cpp \
    b2MotorJoint.cpp
