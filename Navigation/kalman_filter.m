function [x_hat_new, P_new] = kalman_filter(x_hat, P, u, z, F, B, H, Q, R)
    % Prediction step
    x_hat_pred = F * x_hat + B * u;
    P_pred = F * P * F' + Q;

    % Correction step
    K = P_pred * H' / (H * P_pred * H' + R);
    x_hat_new = x_hat_pred + K * (z - H * x_hat_pred);
    P_new = (eye(size(P_pred)) - K * H) * P_pred;
end

