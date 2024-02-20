#include <stdio.h>
#include <iostream>
#include <assert.h>
#include "spider_sense.h" 
using namespace std;

int main() {

	//TEST OF DEFINE LEVEL FUNCTION
    assert(defineLevel(2500) == 4);
    assert(defineLevel(1800) == 3);
    assert(defineLevel(1200) == 2);
    assert(defineLevel(600) == 1);
    assert(defineLevel(200) == 0);

	//TEST OF DEFINE LEVEL FUNCTION
    assert(pwmButton(0) == 252); 
    assert(pwmButton(1) == 189);
    assert(pwmButton(2) == 126);
    assert(pwmButton(3) == 63);
    assert(pwmButton(4) == 0);

    double arr[] = {10.0, 20.0, 30.0, 40.0, 50.0};
    int size = sizeof(arr) / sizeof(arr[0]);
    double m = average1(arr, size);
    double ds = standard_deviation(arr, size, m);
    
    //TEST OF AVERAGE1 FUNCTION
	assert(m == 30.0);
	
	//TEST OF STANDARD_DEVIATION FUNCTION
    assert(ds == 14.142135623730951); 
    
    double arr2[] = {10, 20, 30, 40, 50, 300};
    int size2 = sizeof(arr) / sizeof(arr[0]);
    
    //TEST OF DERIVEHEIGHT FUNCTION
    assert(deriveHeight(arr2, size2) == 30);
    
    //PRINTS
    cout << "LEVEL 2500: correct value: 4 -> result: " << defineLevel(2500) << endl;
    cout << "LEVEL 1800: correct value: 3 -> result: " << defineLevel(1800) << endl;
    cout << "LEVEL 1200: correct value: 2 -> result: " << defineLevel(1200) << endl;
    cout << "LEVEL 600: correct value: 1 -> result: " << defineLevel(600) << endl;
    cout << "LEVEL 200: correct value: 0 -> result: " << defineLevel(200) << endl;
    cout << endl;
    cout << "PWM 0: correct value: 252 -> result: " << pwmButton(0) << endl;
    cout << "PWM 1: correct value: 189 -> result: " << pwmButton(1) << endl;
    cout << "PWM 2: correct value: 126 -> result: " << pwmButton(2) << endl;
    cout << "PWM 3: correct value: 63 -> result: " << pwmButton(3) << endl;
    cout << "PWM 4: correct value: 0 -> result: " << pwmButton(4) << endl;
    cout << endl;
    cout << "AVERAGE: correct value: 30 -> result: " << average1(arr, size) << endl;
	cout << endl;
    cout << "STANDARD DEVIATION: correct value: 14.14 -> result: " << standard_deviation(arr, size, m) << endl;
    cout << endl;
    cout << "HEIGHT: correct value: 30 -> result: " << deriveHeight(arr2, size2) << endl;
    cout << endl;

    printf("ALL TESTS PASSED SUCCESSFULLY!!\n");

    return 0;
}

