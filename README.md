# Muon Tomography


## build

- tools & deps:
  - cmake
  - Eigen
  - magnum(optional)
  - python: mayavi
    ```bash
    pip install mayavi # or conda install mayavi
    ```

- without visualization:
  ```bash
  mkdir build
  cd build
  cmake .. -DBUILD_TESTS=OFF -DBUILD_GRAPHICS=OFF
  ```

## usage

- compute density using PoCA:
```bash
./mtdensity data.txt
```
output is written to result.txt.

- visualization:
  - using the python script
    ```bash
    python visualize.py result.txt
    ```
    
  

