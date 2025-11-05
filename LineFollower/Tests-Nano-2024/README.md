# Line Following Robot Tests - Arduino nano 2024

This directory contains independent test programs for verifying different components of the line following robot.

## Project Structure

Each test is a complete PlatformIO project:

- `01_line_sensors/`: Test line sensor array functionality
- `02_markers/`: Test course marker sensors
- `03_motors/`: Test motor movements and control
- `04_button_led/`: Test button input and LED indicators
- `05_calibration/`: Line sensor calibration utility

## Running Tests

1. Open the desired test folder in VSCode with PlatformIO extension
2. Build and upload the test program:
   ```bash
   cd tests/01_line_sensors  # (or other test directory)
   pio run -t upload
   ```
3. Open Serial Monitor:
   ```bash
   pio device monitor
   ```

## Important Notes

- Each test must be uploaded separately
- Do not try to run multiple tests simultaneously
- Check battery voltage before testing
- Follow the testing procedures in the main documentation

## Adding New Tests

To add a new test:

1. Create a new directory: `XX_test_name/`
2. Copy the platformio.ini from an existing test
3. Create src/main.cpp for your test code
4. Update this README

## Reference

For detailed testing procedures, refer to `docs/testing_guide.md` in the main project repository.