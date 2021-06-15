
#ifndef NUMBERWITHUNITS_HPP_
#define NUMBERWITHUNITS_HPP_

#include <string>
#include <fstream>
#include <iostream>
#include <istream>
#include <map>

namespace ariel {

	//------------------------ DEFINES ------------------------

	using Unit = std::string;

	//------------------------ NUMBER WITH UNITS DECLARATION ------------------------

	class NumberWithUnits {
	public:

		// Constructors
		NumberWithUnits(double _value, const Unit& _unit);

		// Utilities
		static void read_units(std::ifstream& input);

		// Operators
		NumberWithUnits& operator+=(const NumberWithUnits& rhs);
		NumberWithUnits& operator-=(const NumberWithUnits& rhs);
		NumberWithUnits operator+(const NumberWithUnits& rhs) const;
		NumberWithUnits operator-(const NumberWithUnits& rhs) const;
		NumberWithUnits operator+() const;
		NumberWithUnits operator-() const;
		bool operator<(const NumberWithUnits& rhs) const;
		bool operator>(const NumberWithUnits& rhs) const;
		bool operator<=(const NumberWithUnits& rhs) const;
		bool operator>=(const NumberWithUnits& rhs) const;
		bool operator==(const NumberWithUnits& rhs) const;
		bool operator!=(const NumberWithUnits& rhs) const;
		NumberWithUnits& operator++();
		NumberWithUnits operator++(int);
		NumberWithUnits& operator--();
		NumberWithUnits operator--(int);
		NumberWithUnits operator*(double d);

		friend NumberWithUnits operator*(double lhs, const NumberWithUnits& rhs);
		friend std::ostream& operator<<(std::ostream& os, const NumberWithUnits& n);
		friend std::istream& operator>>(std::istream& is, NumberWithUnits& n);

	private:
		double GetConversionValue(const Unit& unitToConvert) const;
		static bool IsUnitValid(const Unit& unit);

		double 	  value;
		Unit      unit;
	};

}

#endif /* NUMBERWITHUNITS_HPP_ */
