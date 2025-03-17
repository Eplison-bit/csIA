#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Meal {
    char foodName[50];
    int calories;
    int protein;
    int carbs;
    int fats;
    int mealType;
};

struct User {
    char name[50];
    int age;
    char gender;
    float height;
    float weight;
    float initialWeight;
    int dailyCalories;
    int remainingCalories;
    int dietType;
    int activityLevel;
    struct Meal meals[20];
    int mealCount;
    int dayCount;
};

void login(struct User *user);
float calculateBMI(float weight, float height);
void adjustCalorieGoal(struct User *user);
void displayMenu(struct User *user);
void processMeal(struct User *user, int mealType);
void saveProgress(struct User *user);
void loadUser(struct User *user);
int displayWeeklySummary(); 
int totalProtein(struct User *user);
int totalCarbs(struct User *user);
int totalFats(struct User *user);

int main() {
    struct User user = { .mealCount = 0, .remainingCalories = 0, .dayCount = 0 };
    loadUser(&user);
    displayMenu(&user);
    return 0;
}

void login(struct User *user) {
    printf("Enter your name: ");
    fgets(user->name, 50, stdin);
    user->name[strcspn(user->name, "\n")] = 0;

    int validInput;
    do {
        validInput = 0;
        printf("Enter your age (1-120): ");
        if (scanf("%d", &user->age) == 1 && user->age >= 1 && user->age <= 120) {
            validInput = 1;
        } else {
            printf("Invalid input. Please enter a number between 1 and 120.\n");
        }
        while (getchar() != '\n');
    } while (!validInput);

    do {
        validInput = 0;
        printf("Enter your gender (M/F): ");
        if (scanf(" %c", &user->gender) == 1 && (user->gender == 'M' || user->gender == 'F')) {
            validInput = 1;
        } else {
            printf("Invalid input. Please enter 'M' or 'F'.\n");
        }
        while (getchar() != '\n');
    } while (!validInput);

    do {
        validInput = 0;
        printf("Enter your height (in meters, 0.5-2.5): ");
        if (scanf("%f", &user->height) == 1 && user->height >= 0.5 && user->height <= 2.5) {
            validInput = 1;
        } else {
            printf("Invalid input. Please enter a number between 0.5 and 2.5.\n");
        }
        while (getchar() != '\n');
    } while (!validInput);

    do {
        validInput = 0;
        printf("Enter your weight (in kg, 20-200): ");
        if (scanf("%f", &user->weight) == 1 && user->weight >= 20 && user->weight <= 200) {
            validInput = 1;
            user->initialWeight = user->weight;
        } else {
            printf("Invalid input. Please enter a number between 20 and 200.\n");
        }
        while (getchar() != '\n');
    } while (!validInput);

    do {
        validInput = 0;
        printf("Activity Level (1=Sedentary, 2=Light, 3=Moderate, 4=Active): ");
        if (scanf("%d", &user->activityLevel) == 1 && user->activityLevel >= 1 && user->activityLevel <= 4) {
            validInput = 1;
        } else {
            printf("Invalid input. Please enter a number between 1 and 4.\n");
        }
        while (getchar() != '\n');
    } while (!validInput);

    do {
        validInput = 0;
        printf("\nSelect your diet type:\n1. Keto\n2. Vegan\n3. Low Carb\n4. Low Fat\n5. Weight Watchers\nChoice: ");
        if (scanf("%d", &user->dietType) == 1 && user->dietType >= 1 && user->dietType <= 5) {
            validInput = 1;
        } else {
            printf("Invalid input. Please enter a number between 1 and 5.\n");
        }
        while (getchar() != '\n');
    } while (!validInput);

    float bmi = calculateBMI(user->weight, user->height);
    printf("\nHello, %s! Your initial BMI is: %.2f\n", user->name, bmi);

    adjustCalorieGoal(user);
}

float calculateBMI(float weight, float height) {
    return weight / (height * height);
}

void adjustCalorieGoal(struct User *user) {
    float bmr;
    if (user->gender == 'M') {
        bmr = 88.362 + (13.397 * user->weight) + (4.799 * user->height * 100) - (5.677 * user->age);
    } else {
        bmr = 447.593 + (9.247 * user->weight) + (3.098 * user->height * 100) - (4.330 * user->age);
    }
    float factors[] = {1.2, 1.375, 1.55, 1.725};
    float calories = bmr * factors[user->activityLevel - 1];
    if (calories > 2147483647) {
        user->dailyCalories = 2147483647;
    } else if (calories < 0) {
        user->dailyCalories = 0;
    } else {
        user->dailyCalories = (int)calories;
    }
    user->remainingCalories = user->dailyCalories;
    printf("Adjusted daily calories: %d (Weight: %.2f kg)\n", user->dailyCalories, user->weight);
}

void displayMenu(struct User *user) {
    int choice;
    printf("Initial remaining calories: %d\n", user->remainingCalories);
    do {
        if (user->dayCount > 0) {
            printf("\nStarting Day %d:\n", user->dayCount + 1);
            int validInput;
            do {
                validInput = 0;
                printf("Activity Level (1=Sedentary, 2=Light, 3=Moderate, 4=Active): ");
                if (scanf("%d", &user->activityLevel) == 1 && user->activityLevel >= 1 && user->activityLevel <= 4) {
                    validInput = 1;
                } else {
                    printf("Invalid input. Please enter a number between 1 and 4.\n");
                }
                while (getchar() != '\n');
            } while (!validInput);
            adjustCalorieGoal(user);
        }

        printf("\n=== Weight Management Menu ===\n");
        printf("1. Breakfast\n2. Lunch\n3. Dinner\n4. Snacks\n5. End Day\n");
        printf("Remaining Calories: %d\n", user->remainingCalories);
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice >= 1 && choice <= 4) {
            processMeal(user, choice);
        } else if (choice == 5) {
            int validInput;
            float newWeight;
            do {
                validInput = 0;
                printf("\nEnter current weight (kg, 20-200): ");
                if (scanf("%f", &newWeight) == 1 && newWeight >= 20 && newWeight <= 200) {
                    validInput = 1;
                } else {
                    printf("Invalid input. Please enter a number between 20 and 200.\n");
                }
                while (getchar() != '\n');
            } while (!validInput);

            if (newWeight < user->initialWeight) printf("You've lost weight!\n");
            else if (newWeight > user->initialWeight) printf("You've gained weight!\n");
            else printf("You're the same weight!\n");

            user->weight = newWeight;

            printf("\n=== End of Day Summary ===\n");
            printf("Name: %s\nAge: %d\nHeight: %.2f m\nWeight: %.2f kg\nBMI: %.2f\n",
                   user->name, user->age, user->height, user->weight, calculateBMI(user->weight, user->height));
            
            int totalP = 0, totalC = 0, totalF = 0;
            printf("\nMeal History:\n");
            for (int i = 0; i < user->mealCount; i++) {
                printf("%s (%s) - %d cal, %dg protein, %dg carbs, %dg fats\n",
                       user->meals[i].foodName,
                       user->meals[i].mealType == 1 ? "Breakfast" :
                       user->meals[i].mealType == 2 ? "Lunch" :
                       user->meals[i].mealType == 3 ? "Dinner" : "Snacks",
                       user->meals[i].calories, user->meals[i].protein,
                       user->meals[i].carbs, user->meals[i].fats);
                totalP += user->meals[i].protein;
                totalC += user->meals[i].carbs;
                totalF += user->meals[i].fats;
            }
            printf("Totals: %d cal, %dg protein, %dg carbs, %dg fats\n",
                   user->dailyCalories - user->remainingCalories, totalP, totalC, totalF);

            saveProgress(user);
            int completedDay = displayWeeklySummary(); 

            printf("\nDay %d completed.\n", completedDay); 
            user->mealCount = 0;
            user->dayCount++; 
        }
    } while (choice != 5);
}

void processMeal(struct User *user, int mealType) {
    char *mealNames[] = {"Breakfast", "Lunch", "Dinner", "Snacks"};
    printf("\n%s Options (Diet Type %d):\n", mealNames[mealType-1], user->dietType);
    
    int foodChoice;
    struct Meal newMeal = { .mealType = mealType };

    switch (user->dietType) {
        case 1: // Keto
            if (mealType == 1) { // Breakfast
                printf("1. Avocado and bacon breakfast burrito (no tortilla) - 350 cal, 15g P, 5g C, 28g F\n");
                printf("2. Spinach and cheese omelet - 240 cal, 18g P, 3g C, 17g F\n");
                printf("3. Keto pancake with almond flour - 300 cal, 10g P, 6g C, 25g F\n");
                printf("4. Greek yogurt with chia seeds and nuts - 280 cal, 12g P, 8g C, 22g F\n");
                printf("5. Smoked salmon and cream cheese roll-ups - 220 cal, 14g P, 2g C, 18g F\n");
                int options[][4] = {{350, 15, 5, 28}, {240, 18, 3, 17}, {300, 10, 6, 25}, {280, 12, 8, 22}, {220, 14, 2, 18}};
                char *names[] = {"Avocado and bacon burrito", "Spinach and cheese omelet", "Keto pancake", "Greek yogurt with nuts", "Salmon roll-ups"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            } else if (mealType == 2) { // Lunch
                printf("1. Grilled chicken salad with avocado - 400 cal, 30g P, 8g C, 28g F\n");
                printf("2. Tuna salad with celery on lettuce wraps - 320 cal, 25g P, 4g C, 22g F\n");
                printf("3. Zucchini noodles with pesto and shrimp - 350 cal, 20g P, 6g C, 27g F\n");
                printf("4. Cauliflower rice bowl with beef - 380 cal, 28g P, 7g C, 26g F\n");
                printf("5. Cobb salad with blue cheese and bacon - 420 cal, 25g P, 5g C, 32g F\n");
                int options[][4] = {{400, 30, 8, 28}, {320, 25, 4, 22}, {350, 20, 6, 27}, {380, 28, 7, 26}, {420, 25, 5, 32}};
                char *names[] = {"Chicken salad with avocado", "Tuna salad lettuce wraps", "Zucchini noodles with shrimp", "Cauliflower rice bowl", "Cobb salad"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            } else if (mealType == 3) { // Dinner
                printf("1. Baked salmon with asparagus - 400 cal, 30g P, 5g C, 28g F\n");
                printf("2. Pork chops with green beans - 380 cal, 32g P, 6g C, 25g F\n");
                printf("3. Stuffed bell peppers with beef - 360 cal, 28g P, 8g C, 24g F\n");
                printf("4. Chicken thighs with mushroom sauce - 420 cal, 30g P, 4g C, 32g F\n");
                printf("5. Ribeye steak with mashed cauliflower - 450 cal, 35g P, 6g C, 33g F\n");
                int options[][4] = {{400, 30, 5, 28}, {380, 32, 6, 25}, {360, 28, 8, 24}, {420, 30, 4, 32}, {450, 35, 6, 33}};
                char *names[] = {"Baked salmon", "Pork chops", "Stuffed peppers", "Chicken thighs", "Ribeye steak"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            } else { // Snacks
                printf("1. Cheese sticks - 80 cal, 6g P, 1g C, 6g F\n");
                printf("2. Nuts and seeds - 200 cal, 5g P, 4g C, 18g F\n");
                printf("3. Olives - 120 cal, 0g P, 2g C, 12g F\n");
                printf("4. Pork rinds - 150 cal, 10g P, 0g C, 12g F\n");
                printf("5. Avocado slices - 160 cal, 2g P, 4g C, 15g F\n");
                int options[][4] = {{80, 6, 1, 6}, {200, 5, 4, 18}, {120, 0, 2, 12}, {150, 10, 0, 12}, {160, 2, 4, 15}};
                char *names[] = {"Cheese sticks", "Nuts and seeds", "Olives", "Pork rinds", "Avocado slices"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            }
            break;

        case 2: // Vegan
            if (mealType == 1) { // Breakfast
                printf("1. Oatmeal with berries and almond butter - 300 cal, 8g P, 45g C, 10g F\n");
                printf("2. Tofu scramble with veggies - 250 cal, 15g P, 12g C, 15g F\n");
                printf("3. Smoothie bowl with fruits and granola - 280 cal, 6g P, 50g C, 5g F\n");
                printf("4. Avocado toast on whole grain - 260 cal, 6g P, 30g C, 12g F\n");
                printf("5. Chia pudding with coconut milk - 270 cal, 5g P, 20g C, 18g F\n");
                int options[][4] = {{300, 8, 45, 10}, {250, 15, 12, 15}, {280, 6, 50, 5}, {260, 6, 30, 12}, {270, 5, 20, 18}};
                char *names[] = {"Oatmeal with berries", "Tofu scramble", "Smoothie bowl", "Avocado toast", "Chia pudding"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            } else if (mealType == 2) { // Lunch
                printf("1. Quinoa salad with black beans - 350 cal, 12g P, 55g C, 8g F\n");
                printf("2. Chickpea and avocado wrap - 320 cal, 10g P, 45g C, 10g F\n");
                printf("3. Lentil soup with spinach - 300 cal, 15g P, 45g C, 5g F\n");
                printf("4. Grilled veggie and hummus sandwich - 310 cal, 10g P, 50g C, 8g F\n");
                printf("5. Buddha bowl with tofu - 340 cal, 14g P, 40g C, 12g F\n");
                int options[][4] = {{350, 12, 55, 8}, {320, 10, 45, 10}, {300, 15, 45, 5}, {310, 10, 50, 8}, {340, 14, 40, 12}};
                char *names[] = {"Quinoa salad", "Chickpea wrap", "Lentil soup", "Veggie sandwich", "Buddha bowl"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            } else if (mealType == 3) { // Dinner
                printf("1. Vegan spaghetti with veggie meatballs - 400 cal, 15g P, 60g C, 10g F\n");
                printf("2. Stuffed sweet potatoes with beans - 380 cal, 12g P, 65g C, 5g F\n");
                printf("3. Veggie stir-fry with tofu - 350 cal, 14g P, 45g C, 12g F\n");
                printf("4. Eggplant lasagna with cashew cheese - 370 cal, 10g P, 40g C, 15g F\n");
                printf("5. Vegan curry with chickpeas - 390 cal, 12g P, 50g C, 15g F\n");
                int options[][4] = {{400, 15, 60, 10}, {380, 12, 65, 5}, {350, 14, 45, 12}, {370, 10, 40, 15}, {390, 12, 50, 15}};
                char *names[] = {"Vegan spaghetti", "Stuffed sweet potatoes", "Veggie stir-fry", "Eggplant lasagna", "Vegan curry"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            } else { // Snacks
                printf("1. Fresh fruit - 100 cal, 1g P, 25g C, 0g F\n");
                printf("2. Veggie sticks with hummus - 120 cal, 3g P, 15g C, 5g F\n");
                printf("3. Trail mix with nuts - 150 cal, 4g P, 10g C, 10g F\n");
                printf("4. Popcorn (no butter) - 90 cal, 2g P, 18g C, 1g F\n");
                printf("5. Almond and date energy balls - 130 cal, 3g P, 15g C, 6g F\n");
                int options[][4] = {{100, 1, 25, 0}, {120, 3, 15, 5}, {150, 4, 10, 10}, {90, 2, 18, 1}, {130, 3, 15, 6}};
                char *names[] = {"Fresh fruit", "Veggie sticks", "Trail mix", "Popcorn", "Energy balls"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            }
            break;

        case 3: // Low Carb
            if (mealType == 1) { // Breakfast
                printf("1. Scrambled eggs with spinach and feta - 250 cal, 20g P, 5g C, 18g F\n");
                printf("2. Greek yogurt with walnuts and berries - 280 cal, 12g P, 10g C, 20g F\n");
                printf("3. Almond flour pancakes - 300 cal, 10g P, 8g C, 25g F\n");
                printf("4. Smoked salmon and avocado on cucumber - 220 cal, 14g P, 4g C, 16g F\n");
                printf("5. Cottage cheese with fresh fruit - 200 cal, 15g P, 10g C, 8g F\n");
                int options[][4] = {{250, 20, 5, 18}, {280, 12, 10, 20}, {300, 10, 8, 25}, {220, 14, 4, 16}, {200, 15, 10, 8}};
                char *names[] = {"Scrambled eggs", "Greek yogurt", "Almond pancakes", "Salmon and avocado", "Cottage cheese"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            } else if (mealType == 2) { // Lunch
                printf("1. Chicken Caesar salad (no croutons) - 350 cal, 30g P, 5g C, 20g F\n");
                printf("2. Turkey and cheese lettuce wraps - 300 cal, 25g P, 4g C, 18g F\n");
                printf("3. Shrimp and avocado salad - 320 cal, 20g P, 6g C, 22g F\n");
                printf("4. Grilled chicken and veggie kabobs - 340 cal, 28g P, 8g C, 20g F\n");
                printf("5. Egg salad on greens - 280 cal, 18g P, 5g C, 20g F\n");
                int options[][4] = {{350, 30, 5, 20}, {300, 25, 4, 18}, {320, 20, 6, 22}, {340, 28, 8, 20}, {280, 18, 5, 20}};
                char *names[] = {"Chicken Caesar", "Turkey wraps", "Shrimp salad", "Chicken kabobs", "Egg salad"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            } else if (mealType == 3) { // Dinner
                printf("1. Baked cod with Brussels sprouts - 320 cal, 25g P, 8g C, 18g F\n");
                printf("2. Beef and broccoli stir-fry - 350 cal, 30g P, 10g C, 20g F\n");
                printf("3. Chicken Parmesan with zucchini noodles - 380 cal, 32g P, 8g C, 22g F\n");
                printf("4. Grilled pork tenderloin with veggies - 340 cal, 28g P, 6g C, 20g F\n");
                printf("5. Lamb chops with spinach and feta - 400 cal, 35g P, 5g C, 25g F\n");
                int options[][4] = {{320, 25, 8, 18}, {350, 30, 10, 20}, {380, 32, 8, 22}, {340, 28, 6, 20}, {400, 35, 5, 25}};
                char *names[] = {"Baked cod", "Beef stir-fry", "Chicken Parmesan", "Pork tenderloin", "Lamb chops"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            } else { // Snacks
                printf("1. Cheese cubes - 100 cal, 7g P, 1g C, 8g F\n");
                printf("2. Celery sticks with peanut butter - 150 cal, 5g P, 6g C, 12g F\n");
                printf("3. Hard-boiled eggs - 80 cal, 6g P, 1g C, 5g F\n");
                printf("4. Bell peppers with guacamole - 120 cal, 2g P, 8g C, 10g F\n");
                printf("5. Mixed nuts - 180 cal, 5g P, 6g C, 15g F\n");
                int options[][4] = {{100, 7, 1, 8}, {150, 5, 6, 12}, {80, 6, 1, 5}, {120, 2, 8, 10}, {180, 5, 6, 15}};
                char *names[] = {"Cheese cubes", "Celery with peanut butter", "Hard-boiled eggs", "Peppers with guac", "Mixed nuts"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            }
            break;

        case 4: // Low Fat
            if (mealType == 1) { // Breakfast
                printf("1. Whole grain toast with banana - 200 cal, 5g P, 40g C, 2g F\n");
                printf("2. Low-fat yogurt with berries - 180 cal, 10g P, 25g C, 2g F\n");
                printf("3. Smoothie with spinach and pineapple - 220 cal, 5g P, 45g C, 1g F\n");
                printf("4. Egg white omelet with veggies - 150 cal, 12g P, 5g C, 2g F\n");
                printf("5. Oatmeal with cinnamon and apples - 190 cal, 6g P, 35g C, 3g F\n");
                int options[][4] = {{200, 5, 40, 2}, {180, 10, 25, 2}, {220, 5, 45, 1}, {150, 12, 5, 2}, {190, 6, 35, 3}};
                char *names[] = {"Toast with banana", "Yogurt with berries", "Smoothie", "Egg white omelet", "Oatmeal with apples"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            } else if (mealType == 2) { // Lunch
                printf("1. Grilled chicken salad with vinaigrette - 300 cal, 25g P, 10g C, 8g F\n");
                printf("2. Turkey and cucumber roll-ups - 250 cal, 20g P, 8g C, 5g F\n");
                printf("3. Minestrone soup with bread - 280 cal, 10g P, 45g C, 5g F\n");
                printf("4. Veggie wrap with hummus - 270 cal, 8g P, 40g C, 6g F\n");
                printf("5. Shrimp and mango salad - 260 cal, 18g P, 20g C, 5g F\n");
                int options[][4] = {{300, 25, 10, 8}, {250, 20, 8, 5}, {280, 10, 45, 5}, {270, 8, 40, 6}, {260, 18, 20, 5}};
                char *names[] = {"Chicken salad", "Turkey roll-ups", "Minestrone", "Veggie wrap", "Shrimp salad"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            } else if (mealType == 3) { // Dinner
                printf("1. Baked chicken with steamed veggies - 320 cal, 30g P, 15g C, 5g F\n");
                printf("2. Salmon with lemon-dill and quinoa - 350 cal, 25g P, 30g C, 8g F\n");
                printf("3. Vegetarian chili with sweet potatoes - 300 cal, 12g P, 50g C, 5g F\n");
                printf("4. Stir-fried tofu with broccoli - 280 cal, 15g P, 25g C, 8g F\n");
                printf("5. Grilled tilapia with couscous - 310 cal, 22g P, 35g C, 6g F\n");
                int options[][4] = {{320, 30, 15, 5}, {350, 25, 30, 8}, {300, 12, 50, 5}, {280, 15, 25, 8}, {310, 22, 35, 6}};
                char *names[] = {"Baked chicken", "Salmon with quinoa", "Vegetarian chili", "Tofu stir-fry", "Grilled tilapia"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            } else { // Snacks
                printf("1. Fresh fruit salad - 100 cal, 1g P, 25g C, 0g F\n");
                printf("2. Low-fat cheese slices - 80 cal, 6g P, 2g C, 3g F\n");
                printf("3. Carrot sticks with hummus - 120 cal, 3g P, 15g C, 5g F\n");
                printf("4. Air-popped popcorn - 90 cal, 2g P, 18g C, 1g F\n");
                printf("5. Rice cakes with almond butter - 130 cal, 3g P, 20g C, 5g F\n");
                int options[][4] = {{100, 1, 25, 0}, {80, 6, 2, 3}, {120, 3, 15, 5}, {90, 2, 18, 1}, {130, 3, 20, 5}};
                char *names[] = {"Fruit salad", "Cheese slices", "Carrot sticks", "Popcorn", "Rice cakes"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            }
            break;

        case 5: // Weight Watchers
            if (mealType == 1) { // Breakfast
                printf("1. Scrambled eggs with spinach and tomatoes - 200 cal, 15g P, 5g C, 12g F\n");
                printf("2. Whole grain toast with avocado - 220 cal, 6g P, 25g C, 10g F\n");
                printf("3. Greek yogurt with berries and honey - 230 cal, 12g P, 20g C, 8g F\n");
                printf("4. Smoothie with banana and spinach - 210 cal, 5g P, 40g C, 2g F\n");
                printf("5. Oatmeal with almond milk and blueberries - 190 cal, 6g P, 35g C, 3g F\n");
                int options[][4] = {{200, 15, 5, 12}, {220, 6, 25, 10}, {230, 12, 20, 8}, {210, 5, 40, 2}, {190, 6, 35, 3}};
                char *names[] = {"Scrambled eggs", "Toast with avocado", "Greek yogurt", "Smoothie", "Oatmeal"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            } else if (mealType == 2) { // Lunch
                printf("1. Turkey and veggie wrap - 300 cal, 20g P, 30g C, 10g F\n");
                printf("2. Grilled chicken salad with vinaigrette - 320 cal, 25g P, 10g C, 12g F\n");
                printf("3. Lentil soup with whole grain bread - 310 cal, 15g P, 45g C, 5g F\n");
                printf("4. Shrimp and avocado salad - 290 cal, 20g P, 15g C, 15g F\n");
                printf("5. Quinoa bowl with roasted veggies - 330 cal, 12g P, 50g C, 8g F\n");
                int options[][4] = {{300, 20, 30, 10}, {320, 25, 10, 12}, {310, 15, 45, 5}, {290, 20, 15, 15}, {330, 12, 50, 8}};
                char *names[] = {"Turkey wrap", "Chicken salad", "Lentil soup", "Shrimp salad", "Quinoa bowl"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            } else if (mealType == 3) { // Dinner
                printf("1. Grilled salmon with asparagus and quinoa - 350 cal, 25g P, 30g C, 12g F\n");
                printf("2. Chicken stir-fry with veggies and rice - 340 cal, 28g P, 35g C, 10g F\n");
                printf("3. Veggie stuffed bell peppers - 300 cal, 10g P, 45g C, 8g F\n");
                printf("4. Baked cod with lemon-caper sauce - 320 cal, 25g P, 10g C, 15g F\n");
                printf("5. Turkey meatballs with spaghetti squash - 310 cal, 22g P, 20g C, 12g F\n");
                int options[][4] = {{350, 25, 30, 12}, {340, 28, 35, 10}, {300, 10, 45, 8}, {320, 25, 10, 15}, {310, 22, 20, 12}};
                char *names[] = {"Grilled salmon", "Chicken stir-fry", "Stuffed peppers", "Baked cod", "Turkey meatballs"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            } else { // Snacks
                printf("1. Fresh fruit slices - 100 cal, 1g P, 25g C, 0g F\n");
                printf("2. Low-fat cheese sticks - 80 cal, 6g P, 2g C, 3g F\n");
                printf("3. Veggie sticks with hummus - 120 cal, 3g P, 15g C, 5g F\n");
                printf("4. Air-popped popcorn - 90 cal, 2g P, 18g C, 1g F\n");
                printf("5. Greek yogurt with honey - 130 cal, 10g P, 15g C, 3g F\n");
                int options[][4] = {{100, 1, 25, 0}, {80, 6, 2, 3}, {120, 3, 15, 5}, {90, 2, 18, 1}, {130, 10, 15, 3}};
                char *names[] = {"Fruit slices", "Cheese sticks", "Veggie sticks", "Popcorn", "Yogurt with honey"};
                do {
                    printf("Choice (1-5): ");
                    scanf("%d", &foodChoice);
                    getchar();
                } while (foodChoice < 1 || foodChoice > 5);
                newMeal.calories = options[foodChoice-1][0];
                newMeal.protein = options[foodChoice-1][1];
                newMeal.carbs = options[foodChoice-1][2];
                newMeal.fats = options[foodChoice-1][3];
                strcpy(newMeal.foodName, names[foodChoice-1]);
            }
            break;
    }

    if (newMeal.calories <= user->remainingCalories && user->remainingCalories >= 0) {
        user->remainingCalories -= newMeal.calories;
        user->meals[user->mealCount] = newMeal;
        user->mealCount++;
        printf("Added: %d calories\n", newMeal.calories);
    } else {
        printf("Not enough calories remaining or invalid calorie state!\n");
    }
}

void saveProgress(struct User *user) {
    FILE *fp = fopen("progress.txt", "a");
    if (fp) {
        fprintf(fp, "%s,%.2f,%d,%d,%d,%d\n", user->name, user->weight,
                user->dailyCalories - user->remainingCalories, totalProtein(user),
                totalCarbs(user), totalFats(user));
        fclose(fp);
    }
}

int totalProtein(struct User *user) {
    int total = 0;
    for (int i = 0; i < user->mealCount; i++) total += user->meals[i].protein;
    return total;
}

int totalCarbs(struct User *user) {
    int total = 0;
    for (int i = 0; i < user->mealCount; i++) total += user->meals[i].carbs;
    return total;
}

int totalFats(struct User *user) {
    int total = 0;
    for (int i = 0; i < user->mealCount; i++) total += user->meals[i].fats;
    return total;
}

void loadUser(struct User *user) {
    FILE *fp = fopen("user_data.txt", "r");
    if (fp && fscanf(fp, "%[^,],%d,%c,%f,%f,%d", user->name, &user->age, &user->gender,
                     &user->height, &user->weight, &user->dietType) == 6) {
        user->initialWeight = user->weight;
        int validInput;
        do {
            validInput = 0;
            printf("Activity Level (1=Sedentary, 2=Light, 3=Moderate, 4=Active): ");
            if (scanf("%d", &user->activityLevel) == 1 && user->activityLevel >= 1 && user->activityLevel <= 4) {
                validInput = 1;
            } else {
                printf("Invalid input. Please enter a number between 1 and 4.\n");
            }
            while (getchar() != '\n');
        } while (!validInput);
        adjustCalorieGoal(user);
        float bmi = calculateBMI(user->weight, user->height);
        printf("\nWelcome back, %s! Your current BMI is: %.2f\n", user->name, bmi);
        fclose(fp);
    } else {
        login(user);
        fp = fopen("user_data.txt", "w");
        if (fp) {
            fprintf(fp, "%s,%d,%c,%.2f,%.2f,%d\n", user->name, user->age, user->gender,
                    user->height, user->weight, user->dietType);
            fclose(fp);
        }
    }
}

int displayWeeklySummary() {
    FILE *fp = fopen("progress.txt", "r");
    if (!fp) return 0;

    char name[50];
    float weight, prevWeight = -1;
    int calories, protein, carbs, fats, days = 0;
    float totalWeightChange = 0;

    printf("\n=== Weekly Summary ===\n");
    while (fscanf(fp, "%[^,],%f,%d,%d,%d,%d\n", name, &weight, &calories, &protein, &carbs, &fats) == 6) {
        if (prevWeight != -1) totalWeightChange += prevWeight - weight;
        printf("Day %d: Weight %.2f kg, %d cal, %dg P, %dg C, %dg F\n",
               ++days, weight, calories, protein, carbs, fats);
        prevWeight = weight;
    }
    if (days > 0) {
        printf("Total weight change: %.2f kg over %d days\n", totalWeightChange, days);
    }
    fclose(fp);
    return days;
}
