nRF52 Preview DK Support
========================

To build got nRF52 Preview DK you need to pass `-DBOARD_PCA10036=1`. For instance:

```
rm -Rf * && cmake -DBOARD=Nordic/nRF52DK -DBOARD_PCA10036=1 -DAPPLICATION=Nordic/ble_app_hrs_peripheral .. && make
```
