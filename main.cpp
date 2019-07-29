//Your goal in this step is a Console Application built in VC++ which has the functionality described here.If you are familiar 
//with Windows programming and it would be easier you can create a windows application instead of a Console App.
//
//Time estimate - about 2 weeks.
//
//
//----1)----
//**Define the classes to be able to construct a 'solid box' with 6 'planes'.No geometry at this point.
//
//**Define a class structure to implement a channel connection between the solid box and its planes.A ‘channel’ 
//		is a logical ‘connection’ between two objects.Simplest implementation of a channel is that each object stores 
//		a pointer to the other object in its instance data.This storage of mutual pointers should be viewed as a 
//		‘connection’.If this implementation were to be abstracted then one could call it a logical ‘channel’ using 
//		which the two objects communicate with each other.The channel protocol should be externalized so that any 
//		object should be able to use it by embedding an object of the channel class in its own instance data.
//		Familiarize yourself with the implications of embedding versus sub - classing for inheritance.
//
//**The channel protocol should include connect, disconnect and cleanup.
//
//**A channel should implement the actual connection via an array of object pointers in its instance data.Keep 
//		the channel many - to - many only for simplicity.Read about canonical class forms.Since the Channel class has 
//		pointers as instance, you will need to provide Default Constructor, Copy Constructor and Assignment Operator 
//		Overload.Read about these C++ concepts.
//
//**Implement a naming scheme so that a unique name is assigned to each instance of Solid and Plane.
//		This could be as simple as a string ‘Cube#’ where the ‘#’ is a unique ever increasing number.
//		Should the names be in a global array ? Read about static instances in C++ in this context.

//**You will need some piece of code to let the user enter a name and then search the list of cubes in memory to 
//		match the name and return the object pointer.This piece of code is essentially in lieu of graphics locate.
//
//----2)----
//**Implement a menu driven main driver that allows following choices -
//
//a)	Create a solid box(should create planes and connect them up to the solid)
//
//b)	Delete a solid box(should perform all memory cleanup and also delete the planes)
//
//c)	Show the set of solids currently in memory.
//
//d)	Copy an existing solid into a new one(although there is no geometry as yet, this should go through the 
//motions of copy.The result should be a new solid from the old solid which in turn means new planes from old planes).
//This should use the C++ copy constructor.
//
//e)	Move one existing solid’s data into another existing solid using the C++ assignment operator. Make sure that 
//when a solid is moved to other solid the previous solid should be removed from the memory.
//
//Operations(d) and (e)above do not include the name of the object.Name remains a unique identifier.The name is your 
//only way of  identifying an object.
//
//f)	Debug a solid(print information about a particular solid - includes going down to planes using the channel 
//connection).
//
//g)	Save the current set of objects to a disk file using the Serialization protocol provided by the Microsoft 
//Class - CObject.
//
//h)	Read an existing disk file into memory - reverse of(g).
//
//The implementation should be such that the planes are not directly accessed after construction.They should be accessed 
//only through the solid object via the channel.


#include "stdafx.h"
#include <vector>
#include <iostream>


int main()
{
	/*std::vector<int> nVec;
	for (int ii = 0; ii < 100; ++ii)
	{
		
		nVec.push_back(ii);
		std::cout << nVec[ii] << " ";
		if ((ii + 1) % 10 == 0)
		{
			std::cout << std::endl;
		}
	}*/
	
    return 0;
}

