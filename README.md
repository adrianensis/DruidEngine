# README

## Build

* First, **download and install dependencies**: `./scripts/dependencies.sh`
* Build project: `./scripts/build.sh -[rtch]`
  * -h Show help.
  * -r Compile Release, **Debug by default**.
  * -t Compile Tests suite. (NEEDED BY `test.sh`)
  * -c Clean the project.
* Run unitary tests: `./scripts/test.sh`
* Generate documentation: `./scripts/doc.sh`

## Directories

* **code:** Source and Headers.
* **scripts:** Contains scripts described above.
* **libOutput:** Contains the libDruid.a. Generated by `scripts/build.sh`.
* **testOutput:** Generated by `scripts/test.sh.`. Contains the test executable files (both unitary and integration).
* **test:** Set of tests. Unitary and Integration.
* **libs:** Generated by `scripts/dependencies.sh.` Contains third party libraries.
* **build:** Generated by `scripts/build.sh`.

## Unitary Tests

Run unitary tests with `./scripts/test.sh`. Result are stored in **testOutput/output**.
This command will print the results in terminal.

## Integration Tests

Run intgration test by executing `./testOutput/test_integration`. Result is stored in **testOutput/output**.

## TODO

* UI
  * Fonts
  * Buttons
  * TextBox
  * Labels
* Matrix - Inverse matrix
* Input - Mouse coordinates - to world space
* Texture Atlas renderer, only for maps. Just an array of tiles (position, collider).
* Map Loader
* Sprite Editor
* Sprite Loader
* Physics Engine
  * Rigid Bodies - Integration / forces
  * Colliders. Use composition instead inheritance.
    * collider->setShape(Box/Sphere)
* Write in files
