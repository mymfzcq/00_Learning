using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace ConsoleApplication4
{
    class Program
    {
        static void Main(string[] args)
        {
            Zoo<Animals> zoo1 = new Zoo<Animals>();
            zoo1.ZOO_ANIMALS.Add(new Animals("animal1"));
            zoo1.ZOO_ANIMALS.Add(new Fish("fish1"));
            zoo1.ZOO_ANIMALS.Add(new Dog("dog1"));
            zoo1.ZOO_ANIMALS.Add(new PetDog("petdog1"));
            zoo1.ZOO_ANIMALS.Add(new Food("food1", "food of fresh meat"));
            zoo1.show();
            Zoo<Dog> zoo2 = zoo1.getpetdog();
            foreach (Dog dog1 in zoo2)
            {
                if (dog1 is PetDog)
                {
                    Console.WriteLine(dog1.Name);
                }
            }
            Console.WriteLine("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
            Zoo<Animals> zoo3 = new Zoo<Animals>();
            zoo3.ZOO_ANIMALS.Add(new Animals("animal3"));
            zoo3.ZOO_ANIMALS.Add(new Fish("fish3"));
            zoo3.ZOO_ANIMALS.Add(new Dog("dog3"));
            zoo3.ZOO_ANIMALS.Add(new PetDog("petdog3"));
            zoo3.ZOO_ANIMALS.Add(new Food("food3", "food of fresh meat"));
            Zoo<Animals> zoo4 = zoo2 + zoo3;
            foreach (Animals a in zoo4)
            {
                Console.WriteLine(a.Name);
            }
            Console.ReadKey();
        }
    }
}