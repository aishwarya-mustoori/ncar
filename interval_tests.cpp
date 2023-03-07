#include <iostream>
#include "TimeInterval.cpp"
#include <vector>
#include <assert.h> 

using namespace std;

/**
 * \brief Divides one time interval by another
 *
 * Given two TimeInterval objects -- a numerator and a denominator -- computes 
 * the quotient (as a long integer) and the remainder (as another TimeInterval)
 * when numerator is divided by denominator.
 *
 */
void divide(const TimeInterval &numerator,
            const TimeInterval &denominator,
            long &quotient,
            TimeInterval &remainder)
{


	quotient = 0;
	remainder = numerator;
	//
	// The simplest algorithm...
	//
	// while (remainder >= denominator) {
	// 	remainder -= denominator;
	// 	quotient++;
	// }

	//
	// Are there edge cases that aren't handled by the simple algorithm?
	// 1. What if one of the numerator or denominator or both is Negative Interval?
	// 2. Overflow - denominator = 0 etc
	// 3. Takes a long time to execute as it considers O(denominator)
	//


	//
	// Are there algorithms that can arrive at the answer much more efficiently?
	// Yes - O(logn) Both Space and time
	// Logic of the algorithm - Added in ReadME
	//
	//

	// HALF of Minimum value for long to remove overflows
	TimeInterval HALF_LONG_MIN = TimeInterval((LONG_MIN/2),0,0);
	
	// Added - Case Edge Case 1 (Optimizing case where numerator = denominator)
	if(numerator == denominator){
		remainder = TimeInterval(0,0,0);
		quotient = 1;
		return;
	}

	TimeInterval dividend = numerator;
	remainder = denominator;	
	
	// Added - Case Edge Case 2 (denominator == 0,0,0) 

	if(denominator == TimeInterval(0,0,0)){
		return;
	}

	// Added - Case Edge Case 3 (Optimizing case where numerator = 0,0,0 condition)

	if(numerator == TimeInterval(0,0,0)){
		remainder = numerator;
		return;
	}



	// Making both dividend and remainder(denominator) negative if positive
	// Storaging the sign in negatives, just a flag to store whether one dividend/remainder was positive
    int negatives = 2;
    if (dividend > TimeInterval(0,0,0)) {
        negatives-- ;
		// As dividend is positive, subtracting 2*dividend to make it negative
        dividend -= (dividend*2);
    }
    if (remainder > TimeInterval(0,0,0)) {
        negatives--;
		// As remainder is positive, subtracting 2*remainder to make it negative
        remainder -= (remainder*2);
    }
	
    std::vector<TimeInterval> doublesPower;
    std::vector<long> powers;

    long powerIndex = -1;
	
	//Now as both dividend and remainder are negative,We consider negative powers of two -1,-2,-3
	// in powers vector, we store powers[i] = -i
	// For each powerOfTwo[i], doubling the value of denominator and we store in each step.
    while (remainder >= dividend) {
        doublesPower.push_back(remainder);
        powers.push_back(powerIndex);
     
        if (remainder < HALF_LONG_MIN) {
            break;
        }
        remainder += remainder;
   		powerIndex += powerIndex;     
    }

	// Iterate through double vector from largest to smallest, checking if the current double value fits  into the dividend 
    for (int i = doublesPower.size() - 1; i >= 0; i--) {

        if (doublesPower[i] >= dividend) {
			// When it fits 
			//Updating quotient by adding current powerOfTwo to the quotient.
            quotient += powers[i];
			//Update dividend to take into account the double we currently removed,
            dividend -= doublesPower[i];
        }
    }
	
	// Change Sign of quotient(if numerator and denominator have different sign)
    if (negatives != 1) {
        quotient= -1*(quotient);
		
    } 

	// Change sign of remainder, if we changed the sign of dividend or remainder
	if(negatives == 2){
		remainder = dividend;
	}	
	else  {
		remainder = (-1)*dividend;
			
	}
   return;
}


int main(int argc, char **argv)
{
	TimeInterval numerator;
	TimeInterval denominator;
	TimeInterval remainder;
	long quotient;

	//
	// Add tests here to verify the correctness of your implementation
	// of interval division
	//

	// Test 1 - Test case where numerator and denominator are both Positive Time Intervals and remainder is  zero 
	numerator.setInterval(500, 0, 200000);    // 500 days
	denominator.setInterval(0, 0, 200000);    // 0.2 seconds

	divide(numerator, denominator, quotient, remainder);

	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;

	assert((quotient*denominator)+remainder==numerator);



	// Test 2 - Test case where numerator and denominator are both Positive Time Intervals and remainder is not zero 
	numerator.setInterval(500, 0, 200000);    // 500 days 0.2 seconds
	denominator.setInterval(1, 0, 200000);    // 1 day 0.2 seconds

	divide(numerator, denominator, quotient, remainder);

	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;

	assert((quotient*denominator)+remainder==numerator);

	// Test 3 - Test Case for Testing All-Zero time interval as numerator
	numerator.setInterval(0, 0 , 0);    // 0 days
	denominator.setInterval(2, 0, 0);    // 2 Days
	divide(numerator, denominator, quotient, remainder);

	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;
	
	assert((quotient*denominator)+remainder==numerator);

	// Test 4 - Test Case for testing numerator==denominator with numerator and denominator being Positive Time Intervals
	numerator.setInterval(3, 0 , 0);    // 3 Days
	denominator.setInterval(3, 0, 0);    // 3 Days
	divide(numerator, denominator, quotient, remainder);

	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;
	
	assert((quotient*denominator)+remainder==numerator);



	// Test 5 - Test Case  with numerator and denominator being large positive Time Intervals
	numerator.setInterval(4000000, 2000 , -1000);    // 4000000 days, 2000 seconds, -1000 milliseconds
	denominator.setInterval(50000, 10, 10);   // 50000 days, 10 seconds, 10 milliseconds
	divide(numerator, denominator, quotient, remainder);

	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;
	
	assert((quotient*denominator)+remainder==numerator);

	// Test 6 - Test Case  with denominator > numerator
	numerator.setInterval(4000000, 2000 , -1000);    // 4000000 days, 2000 seconds, -1000 milliseconds
	denominator.setInterval(5000000, -10, -10);   // 50000 days, -10 seconds, -10 milliseconds
	divide(numerator, denominator, quotient, remainder);

	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;
	
	assert((quotient*denominator)+remainder==numerator);


	// From here, the simple algorithm either takes a lot of time or doesnt execute

	// Test 7 - Test case for testing big numerator time interval with both numerator and denominator are both positive Time Intervals
	numerator.setInterval(2147483647, 86399 , 999999);    // 2147483647 days
	denominator.setInterval(0, 8639, 0);    // 8639  seconds

	divide(numerator, denominator, quotient, remainder);

	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;
	cout << quotient*denominator;

	assert((denominator*quotient)+remainder==numerator);


	// Test 8 - Test Case  with denominator being large negative time interval
	numerator.setInterval(4000000, 2000 , -1000);    // 4000000 days, 2000 seconds, -1000 milliseconds
	denominator.setInterval(-50000, -10, -10);   // -50000 days, -10 seconds, -10 milliseconds
	divide(numerator, denominator, quotient, remainder);

	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;
	
	assert((quotient*denominator)+remainder==numerator);


	// Test 9 - Test Case for testing numerator==denominator with numerator and denominator being negative intervals
	numerator.setInterval(-3, -10 , -10);    // -3 days, -10 seconds, -10 milliseconds
	denominator.setInterval(-3, -10, -10);   // -3 days, -10 seconds, -10 milliseconds
	divide(numerator, denominator, quotient, remainder);

	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;
	
	assert((quotient*denominator)+remainder==numerator);



	// Test 10 - Test case for testing big time intervals with positive numerator time interval and negative denominator time interval
	numerator.setInterval(2147483647, 0 , 0);  // 2147483647 days
	denominator.setInterval(-1, 0, 0);    // -1 Days

	divide(numerator, denominator, quotient, remainder);

	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;
	
	assert((quotient*denominator)+remainder==numerator);


	// Test 11 - Test Case where numerator and denominator are negative time intervals
	numerator.setInterval(-2147483647, 0 , 0);    // -2147483647 days
	denominator.setInterval(-1, 0, 0);    // -1 Days
	divide(numerator, denominator, quotient, remainder);

	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;
	
	assert((quotient*denominator)+remainder==numerator);	



	// Test 12 - Test Case  where numerator is a negative time interval and denominator is positive interval
	numerator.setInterval(-2147483648, 0 , 0);    // -2147483647 days, -86399 seconds, -999999 milliseconds
	denominator.setInterval(2, 0, 0);    // 2 Days
	divide(numerator, denominator, quotient, remainder);

	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;
	
	assert((quotient*denominator)+remainder==numerator);



	// Test 13 - Another Test Case where numerator is a negative time interval and denominator is positive interval
	numerator.setInterval(-2, 0 , 0);    // -2 days
	denominator.setInterval(2, 0, 0);    // 2 Days
	divide(numerator, denominator, quotient, remainder);

	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;
	
	assert((quotient*denominator)+remainder==numerator);



	// Test 14 - Test Case for Testing All-Zero time interval as denominator
	numerator.setInterval(-2, 0 , 0);    // -2 days
	denominator.setInterval(0, 0, 0);    // 0 Days
	divide(numerator, denominator, quotient, remainder);

	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;
	
	assert(quotient==0);
	assert(remainder==TimeInterval(0,0,0));


	// Test 15 - Test Case for testing maximum negative
	numerator.setInterval(-2147483648, 0 , 0);    // -2147483648 days, -86399 seconds, -999999 milliseconds
	denominator.setInterval(2, 0, 0);    // 2 Days
	divide(numerator, denominator, quotient, remainder);

	cout << (quotient*denominator);
	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;
	
	assert((quotient*denominator)+remainder==numerator);

	
	// Test 16 - Test case for testing maximum negative boundaries with the smallest positive denominator time interval possible[Performance Test 1]
	numerator.setInterval(-2147483648, -86399 , -999999);    // -2147483647 days, -86399 seconds, -999999 milliseconds
	denominator.setInterval(0, -8640, 0);    // -8640 second

	divide(numerator, denominator, quotient, remainder);

	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;
	
	assert((quotient*denominator)+remainder==numerator);


	// Test 17 - Test case for testing long boundaries with the largest negative denominator time interval possible[Performance Test 2]
	numerator.setInterval(2147483647, 86399 , 999999);    // 2147483647 days, 86399 seconds, 999999 milliseconds
	denominator.setInterval(-2147483648, -86399 , -999999);     // -2147483648 days, -86399 seconds, -999999 milliseconds

	divide(numerator, denominator, quotient, remainder);

	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << endl;
	
	assert((quotient*denominator)+remainder==numerator);

	cout << "Tests Ran Successfully"<<endl;
	return 0;
}

