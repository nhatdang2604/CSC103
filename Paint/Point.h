#pragma once
#ifndef  _POINT_H_
#define _POINT_H_

#include<string>
#include<fstream>

namespace figure{
	class Point {

	private://private attribute
		int _x;
		int _y;

	public:	//getter and setter
		int x() const;
		int y() const;
		void setX(int x);
		void setY(int y);
		
	public:	//static method
		static Point parse(std::string&, std::string = ",");

	public:	//overload operator
		friend std::ostream& operator<<(std::ostream&, const Point&);

	public:	//constructor and destructor
		Point();
		Point(int, int);
		~Point();
	};
};

#endif // ! _POINT_H_




