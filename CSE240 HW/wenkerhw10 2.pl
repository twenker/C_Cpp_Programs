/* Todd Wenker
   CSE240 Chen
   11/29/14*/

/*Facts*/
maindish(thanksgiving, turkey).
maindish(easter, ham).
maindish(thanksgiving, duck).
maindish(christmas, roastbeef).
maindish(christmas, pheasant).

sidedish(thanksgiving, rolls).
sidedish(thanksgiving, mashedpotatoes).
sidedish(thanksgiving, stuffing).
sidedish(thanksgiving, greenbeans).
sidedish(easter, yams).
sidedish(easter, peas).
sidedish(easter, deviledeggs).
sidedish(christmas, squash).
sidedish(christmas, salad).
sidedish(christmas, artichokes).

ingredient(rolls, flour). ingredient(rolls, water).
ingredient(mashedpotatoes, potatoes). ingredient(mashedpotatoes, salt).
ingredient(stuffing, bread). ingredient(stuffing, seasoning).
ingredient(greenbeans, greenbeans). ingredient(greenbeans, butter).
ingredient(yams, yams). ingredient(yams, brownsugar).
ingredient(peas, peas). ingredient(peas, salt).
ingredient(deviledeggs, eggs). ingredient(deviledeggs, paprika).
ingredient(squash, squash). ingredient(squash, butter).
ingredient(salad, lettuce). ingredient(salad, dressing).
ingredient(artichokes, artichokes). ingredient(artichokes, garlic).
ingredient(bread, flour). ingredient(bread, flour).

/* Rules */
holidaymeal(Holiday, Food) :-
        maindish(Holiday, Food),
        sidedish(Holiday, _).

shoppinglist(Holiday, Ingredient) :-


