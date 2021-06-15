#include "NumberWithUnits.hpp"
#include <map>
#include <math.h>

using namespace ariel;

//------------------------ GLOBALS ------------------------

static std::map<Unit, std::map<Unit, double> > units_table;

//------------------------ NUMBER WITH UNITS STATIC FUNCTIONS ------------------------

void NumberWithUnits::read_units(std::ifstream& input)
{
	if (input.is_open()) {
		std::string line;
		while (getline(input, line)) {
			size_t endOfFirstUnit = line.find(' ', 2);
			Unit unit1 = line.substr(2, endOfFirstUnit - 2);
			size_t startOfValue = endOfFirstUnit + 3,
				   endOfValue = line.find(' ', startOfValue);
			std::string value_string = line.substr(startOfValue, endOfValue - startOfValue);
			double value = std::stod(value_string);
			Unit unit2 = line.substr(endOfValue + 1, line.size() - endOfValue);

			units_table[unit1][unit2] = value; 
            units_table[unit2][unit1]= 1/value; 
            
            for (const auto& pair : units_table[unit1])
            {
                double tmp = units_table[unit2][unit1] * pair.second;
                units_table[unit2][pair.first] = tmp;
                units_table[pair.first][unit2] = 1/tmp;            
            }

            for (const auto& pair : units_table[unit2])
            {
                double tmp = units_table[unit1][unit2] * pair.second;
                units_table[unit1][pair.first] = tmp;
                units_table[pair.first][unit1] = 1/tmp;
            }
		}
		input.close();
	}
}

//------------------------ NUMBER WITH UNITS DEFINITION ------------------------

// Public methods

NumberWithUnits::NumberWithUnits(double _value, const Unit& _unit)
{
	auto iterPair = units_table.equal_range(_unit);
	if (iterPair.first == iterPair.second) {
		throw std::runtime_error("Unit is not legal. Can't init object.");
	}
	value = _value;
	unit = _unit;
}

NumberWithUnits& NumberWithUnits::operator+=(const NumberWithUnits& rhs)
{

	double conversion_val = GetConversionValue(rhs.unit);
	value += rhs.value * conversion_val;
	return *this;
}

NumberWithUnits& NumberWithUnits::operator-=(const NumberWithUnits& rhs)
{

	double conversion_val = GetConversionValue(rhs.unit);
	value -= rhs.value * conversion_val;
	return *this;
}

NumberWithUnits NumberWithUnits::operator+() const
{
	return *this;
}
NumberWithUnits NumberWithUnits::operator-() const
{
	return NumberWithUnits(-value, unit);
}

NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits& rhs) const
{
	return NumberWithUnits(value, unit).operator +=(rhs);
}

NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits& rhs) const
{
	return NumberWithUnits(value, unit).operator -=(rhs);
}

bool NumberWithUnits::operator<(const NumberWithUnits& rhs) const
{
	double conversion_val = GetConversionValue(rhs.unit);
	return rhs.value * conversion_val - value > 0;
}

bool NumberWithUnits::operator>(const NumberWithUnits& rhs) const
{
	return rhs < *this;
}

bool NumberWithUnits::operator<=(const NumberWithUnits& rhs) const
{
	return (*this < rhs) || (*this == rhs);
}
bool NumberWithUnits::operator>=(const NumberWithUnits& rhs) const
{
	return (*this > rhs) || (*this == rhs);
}

bool NumberWithUnits::operator==(const NumberWithUnits& rhs) const
{
	const double epsilon = 0.000001;
	double conversion_val = GetConversionValue(rhs.unit);
	return std::abs(value - rhs.value * conversion_val) < epsilon;
}

bool NumberWithUnits::operator!=(const NumberWithUnits& rhs) const
{
	return !operator==(rhs);
}

NumberWithUnits& NumberWithUnits::operator++()
{
	value += 1;
	return *this;
}

NumberWithUnits NumberWithUnits::operator++(int)
{
	NumberWithUnits old = *this;
	operator++();
	return old;
}

NumberWithUnits& NumberWithUnits::operator--()
{
	value -= 1;
	return *this;
}

NumberWithUnits NumberWithUnits::operator--(int)
{
	NumberWithUnits old = *this;
	operator--();
	return old;
}

NumberWithUnits NumberWithUnits::operator*(double d)
{
	return NumberWithUnits(d * value, unit);
}

// Friend methods

NumberWithUnits ariel::operator*(double lhs, const NumberWithUnits& rhs)
{
	return NumberWithUnits(lhs * rhs.value, rhs.unit);
}

std::ostream& ariel::operator<<(std::ostream& os, const NumberWithUnits& n)
{
	os << n.value << "[" << n.unit << "]";
	return os;
}

std::istream& ariel::operator>>(std::istream& is, NumberWithUnits& n)
{
	double value = 0;
	std::string unit;
	char ch = ' ';
	is >> value;
	is >> ch ;
	while (ch != ']'){
		if (ch != '['){
			unit.insert(unit.end(), ch);
		}
		is >> ch;
	}

	if (!NumberWithUnits::IsUnitValid(unit))
	{
		throw std::runtime_error("Unit is not legal.");
	}
	
	n.value = value;
	n.unit = unit;
	return is;
}

// Private methods

double NumberWithUnits::GetConversionValue(const Unit& unitToConvert) const
{
	if (unit == unitToConvert) {
		return 1;
	}

	if (units_table[unit].count(unitToConvert) == 0)
	{
		throw std::runtime_error("Unit is not legal.");
	}
	return units_table[unitToConvert][unit];
}

bool NumberWithUnits::IsUnitValid(const Unit& unit)
{
	return !units_table[unit].empty(); 
}
