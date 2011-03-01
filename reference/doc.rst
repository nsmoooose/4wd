Some documentation
==================

RigidBody
---------

Having a constraint with a spring can make the object never
stop swaying. I imagined setting damping on the spring would
have the desired effect of letting the object slowly stop
swaying. By setting it on the rigid body instead works.
Values are in the range 0.0 -> 1.0.
setDamping(linear, angular)

Constraints
-----------

* btConeTwistConstraint

* btContactConstraint

* btGeneric6DofConstraint

* btHingeConstraint

* btPoint2PointConstraint

* btSliderConstraint
  Slide in x axes only. Remember if you want to have an other axis
  you can set the getBasis().setEulerZYX() on both the reference
  frames.

  Having a btSliderConstraint on a wheel as suspension isn't really
  working since you rotate the wheel around the x axis and want to
  have translation in the z axes.

* btGeneric6DofSpringConstraint <- btGeneric6DofConstraint

* btHinge2Constraint <- btGeneric6DofSpringConstraint

* btUniversalConstraint




applyTorqueImpulse
  Sätter direkt förändring av rotationell hastighet direct.

applyTorque
  Ackumulerar rotationell hastighet inför nästa simuleringssteg.
