# Tower of London test

This is our team's project at Dragonhack 2016.

## About Tower of London

The Tower of London problem consists of four balls, each a different colour, and three pegs. The
patient is given the beginning state of balls and the end state; his goal is to get from the
beginning state to the end state using as little moves as possible. The only constraint on the moves
is the natural one, as we can only move the top ball of any peg, onto a peg which is not yet full.

Since its invention in 1982, the Tower of London test has often been used to assess patient's
**planning ability**.

## About us

Our goal is to make the already established psychological test Tower of London more accessible and
easy-to-use for patients that may find a computer mouse hard to control. The programme tracks the
patient's hand movements, making use of the Myo armband. The results of the test are then shown and
analysed in this web application.

### The JIMM team consists of:

*    Jure Slak
*    Ines Meršak
*    Marko Ljubotina
*    Miloš Ljubotina

## Our work

We used MYO-controlled C++ graphical user interface to perform the test
for people that cannot use the mouse or computers properly.
After each test  data is logged onto the web server, stored in the database and
displayed to the patient and the supervisor. They can see the results, the moves they made
and visualize and follow the optimal solution.

Using MYO we can also track the shaking of an arm, and we store that as well.
We also have a page to report statistics and monitor the progress of the disease.

## To try it out
Compile the `main.cpp` into an executable named `jimm`. You will need `SMFL` and `MYO SKD`.

The, simply run `server.py` and you are all set.
