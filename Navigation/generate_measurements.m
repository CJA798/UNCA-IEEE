function z = generate_measurements(x, noise_std)
    % Generate measurement matrix
    z = x + noise_std * randn(size(x));
end
