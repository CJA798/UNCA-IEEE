Radius = 2.5; % (cm)
Circumference = Radius*2*pi; %(cm)
StepsPerRadMtr = 200 * 2;%steps*(1/microstep)
StepsPercm = StepsPerWheelRotation / Circumference;
StepsPerWheelRad = StepsPerWheelRotation / (2*pi);
wheeldist = 17.2;% Distance from wheels center (contact point with ground) to the center of the bot.
BotCircumference = wheeldist*2*pi; %(cm/rotation)
StepsPerBotRad = ((BotCircumference*StepsPercm)/(2*pi))*3
StepsPerDistMult = (1/sqrt(2)*StepsPercm)*3
STEPSPERBOTRAD= (13750.98);
STEPSPERDISTMULT= (432.1518) ;

Vx = 0; % Speed in X
Vy = 0; % Speed in Y
Vomega = 10; % Rotational Speed
Trans = [-wheeldist 1/sqrt(2) 1/sqrt(2);    %Jacobian Matrix (inverse kinematics)
         -wheeldist 1/sqrt(2) -1/sqrt(2);
         -wheeldist -1/sqrt(2) 1/sqrt(2);
         -wheeldist -1/sqrt(2) -1/sqrt(2)];

%Calculation of wheel speeds using new method
V = [Vomega ; Vx; Vy];
WheelSpeeds = (1/Radius)*(Trans*V)


%%
stepsperrad= 13750.98;
stepsperdist= 432.1518 ;
FullRotSteps= -34560
Vx = 1; % Speed in X
Vy = 0; % Speed in Y
Vomega = 0; % Rotational Speed
X = Vx;
Y = Vy; 

R = sqrt( Vx.^2 + Vy.^2);

XYVectorAngle = atan(Y/X);


X = R * cos( pi/2 - XYVectorAngle);
Y = R * sin(-XYVectorAngle);


Trans2 = [-stepsperrad stepsperdist stepsperdist;    %Jacobian Matrix (inverse kinematics)
         -stepsperrad stepsperdist -stepsperdist;
         -stepsperrad -stepsperdist stepsperdist;
         -stepsperrad -stepsperdist -stepsperdist];

%Calculation of wheel speeds using new method
V = [Vomega ; Vx; Vy]
V2 = [Vomega ; X; Y]
WheelSpeeds = (Trans2*V)
WheelSpeeds2 = (Trans2*V2)



