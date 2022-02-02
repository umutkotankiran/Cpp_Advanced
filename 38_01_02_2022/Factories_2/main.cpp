#include "application.h"
#include <iostream>

int main()
{
	Application ax;

	ax.NewDocument("xml");
	ax.PrintDocument();
	ax.SaveDocument();
	

	//ax.NewDocument("pdf");
	//ax.PrintDocument();
	//ax.SaveDocument();
	 
	// //ax.NewDocument("xml");
	//ax.PrintDocument();
	//ax.SaveDocument();

}