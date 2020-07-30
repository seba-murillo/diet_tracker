# DietTracker

DietTracker is a color-CLI nutritional tracking log

## Installation

Simply use the makefile by typing:

```bash
make
```

## Usage

To run DietTracker just run it like any other:
```bash
./diet_tracker
```

To add the amount 'A' of the food _foodname_ to the day's log use:
```bash
add foodname A
```
-> the available foods can be expanded by editing food_data.ods with any spreadsheet software


To add the amount 'A' of the food _foodname_ to the day's log use:
```bash
ls
```

To show the program's help use:
```bash
help
```
other comands include:
```python
load 'DD/MM' or 'DD/MM/YYYY'
set 'foodname' 'amount'
delete 'foodname'
info 'foodname'
last 'days'
average 'days'
profile
weights
clear
exit
```