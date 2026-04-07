## Build Dependencies

### Steamworks SDK

This project requires the Steamworks SDK, which is proprietary and must be downloaded separately from Valve's website:

https://partner.steamgames.com/downloads/steamworks_sdk

Place the SDK files in `libs/steam`.

The SDK is not included in this repository due to licensing restrictions.

### Rrespacker

This project requires Rrespacker, which is proprietary software and must be obtained separately:

https://raylibtech.itch.io/rrespacker

Place Rrespacker files in `client/libs/rrespacker`.

Rrespacker is not included in this repository due to licensing restrictions.

## Building from source

```bash
git clone --recurse-submodules https://github.com/Liam-Weitzel/Roche-Limit.git
cd Roche-Limit/client
mkdir build
cd build
cmake ..
make
cd ..
./prep_models
./client
```
