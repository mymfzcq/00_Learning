using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace ConsoleApplication4
{
    public class Animals
    {
        public Animals(string s)
        {
            Name = s;
        }
        public string Name
        { get; set; }
        public virtual void eat()
        {
        }
    }
    public class Fish : Animals
    {
        public Fish(string a)
            : base(a)
        {
        }
        public override void eat()
        {
            bubble();
        }
        public void bubble()
        {
            Console.WriteLine("fish bubble");
        }
    }
    public class Dog : Animals
    {
        public Dog(string a)
            : base(a)
        {
        }
        public override void eat()
        {
            bite();
        }
        public virtual void bite()
        {
            Console.WriteLine("dog bite");
        }
    }
    class Food : Animals
    {
        public Food(string a, string b)
            : base(a)
        {
            foodforfood = b;
        }
        public bool status;
        public string foodforfood;
        public override void eat()
        {
            if (status)
            {
                feedself(foodforfood);
                killself();
            }
            else
            {
                throw new System.ArgumentOutOfRangeException();
            }
        }
        public void feedself(string f)
        {
            Console.WriteLine("feed the food animal using {}", f);
        }
        public void killself()
        {
            Console.WriteLine("food animal is dead");
        }
    }
    public class PetDog : Dog
    {
        Animals food
        { get; set; }
        public PetDog(string a)
            : base(a)
        {
        }
        public override void eat()
        {
            feed();
            bite();
        }
        public override void bite()
        {
            Console.WriteLine("petdog bite");
        }
        public void feed()
        {
            switch (food.GetType().ToString())
            {
                case "Food":
                    {
                        break;
                    }
                default:
                    break;
            }
        }
    }
    public class Zoo<T> : IEnumerable<T> where T : Animals
    {
        public List<T> zoo_animals = new List<T>();
        public List<T> ZOO_ANIMALS
        {
            get
            {
                return zoo_animals;
            }
        }
        public IEnumerator<T> GetEnumerator()
        {
            return zoo_animals.GetEnumerator();
        }
        IEnumerator IEnumerable.GetEnumerator()
        {
            return zoo_animals.GetEnumerator();
        }
        public void show()
        {
            foreach (T animal in zoo_animals)
            {
                Console.WriteLine(animal.Name + "is showing");
            }
        }
        public Zoo<Dog> getpetdog()
        {
            Zoo<Dog> zoo_petdog = new Zoo<Dog>();
            foreach (T animal in zoo_animals)
            {
                if (animal is Dog)
                {
                    zoo_petdog.ZOO_ANIMALS.Add(animal as Dog);
                }
            }
            return zoo_petdog;
        }
        //public static implicit operator Zoo<Animals>(Zoo<T> z)
        //{
        //    Zoo<Animals> result = new Zoo<Animals>();
        //    foreach(T animal in z)
        //    {
        //        result.ZOO_ANIMALS.Add(animal);
        //    }
        //    return result;
        //}
        //public static Zoo<T> operator +(Zoo<T> z1, Zoo<T> z2)
        //{
        //    Zoo<T> result = new Zoo<T>();
        //    foreach (T animal in z1)
        //    {
        //        result.ZOO_ANIMALS.Add(animal);
        //    }
        //    foreach (T animal in z2)
        //    {
        //        if (!result.ZOO_ANIMALS.Contains(animal))
        //        {
        //            result.ZOO_ANIMALS.Add(animal);
        //        }
        //    }
        //    return result; 
        //}
        public static implicit operator List<Animals>(Zoo<T> farm)
        {
            List<Animals> result = new List<Animals>();
            foreach (T animal in farm)
            {
                result.Add(animal);
            }
            return result;
        }
        public static Zoo<T> operator +(Zoo<T> farm1, List<T> farm2)
        {
            Zoo<T> result = new Zoo<T>();
            foreach (T animal in farm1)
            {
                result.ZOO_ANIMALS.Add(animal);
            }
            foreach (T animal in farm2)
            {
                if (!result.ZOO_ANIMALS.Contains(animal))
                {
                    result.ZOO_ANIMALS.Add(animal);
                }
            }
            return result;
        }
        public static Zoo<T> operator +(List<T> farm1, Zoo<T> farm2)
        {
            return farm2 + farm1;
        }
    }
}