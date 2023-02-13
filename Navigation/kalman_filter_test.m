%% Kalman Filter

% Clear variables and console
clc
clear all

% Define time variables
dt = 0.01; % Sampling period
T = 200; % Length of simulation
t = 0:dt:T; % Time vector

% Define position, velocity and acceleration
position = -sin(t) + t; % Initial position
velocity = -cos(t)+1; % Initial velocity
acceleration = sin(t); % Initial acceleration

% Define state variables
x = [position; velocity; acceleration]; % State vector
dim_x = 3; % Number of state variables

% Define initial state and state estimate
x_init = [0; 0; 0]; % Initial state
x_hat_init = [0; 0; 0]; % Initial state estimate

% Define system model
F = [1 dt 0.5*dt^2; 0 1 dt; 0 0 1]; % State transition matrix
B = [0; 0; 0]; % Control input matrix

% Define measurement model
H = [0 0 1]; % Measurement matrix

% Define process noise covariance matrix
Q = [0.1 0 0; 0 0.1 0; 0 0 0.1];

% Define measurement noise covariance matrix
R = 1;

% Define initial covariance matrix
P_init = eye(dim_x); % Identity matrix

% Define control input
u = 0; % No control input

% Generate measurement noise matrix
z = generate_measurements(acceleration, 0.05);

% Initialize Kalman filter
[x_hat_new, P_new] = kalman_filter(x_hat_init, P_init, u, z(1), F, B, H, Q, R);
x_hat = zeros(1,length(t));

% Iterate Kalman filter for each time step
for n = 2:length(t)
    % Update state estimate and covariance matrix
    [x_hat_new, P_new] = kalman_filter(x_hat_new, P_new, u, z(n), F, B, H, Q, R);
    x_hat(1,n) = x_hat_new(1);
    x_hat(2,n) = x_hat_new(2);
    x_hat(3,n) = x_hat_new(3);
end

% Plot results
figure()
plot(t, x(3,:), 'b-', t, x_hat(3,:), 'r-', t, z, 'g-');
legend('Actual', 'Estimated', 'Measured');
figure()
plot(t, x(2,:), 'b-', t, x_hat(2,:), 'r-');
legend('Actual', 'Estimated');
figure()
plot(t, x(1,:), 'b-', t, x_hat(1,:), 'r-');
legend('Actual', 'Estimated');
