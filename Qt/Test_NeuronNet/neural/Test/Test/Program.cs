using System;
using System.Collections;
using System.Collections.Generic;

using System.Web.Script.Serialization;
using System.Windows.Forms;

namespace Test
{
	//------------------------------------------------------
	class new_test
	{
		int x = 0;
		public String get()
		{
			return x.ToString();
		}

		public String get_str()
		{
			return "x=";
		}

		public new_test(int value)
		{
			x = value;
		}
	}
	//------------------------------------------------------
	class MainClass
	{
		private static void test()
		{
			int[,] arr = new int[15, 5];

			int max_x = arr.GetLength(0);
			int max_y = arr.GetLength(1);
			Console.WriteLine("x=" + max_x);
			Console.WriteLine("y=" + max_y);
			for (int y = 0; y<max_y; y++)
			{
				for (int x = 0; x<max_x; x++)
				{
					arr[x, y] = 0;
				}
			}
		}

		private static void test2()
		{
			List<new_test> ta = new List<new_test>();
			for (int i = 0; i < 10; i++)
			{
				ta.Add(new new_test(i));
			}

			foreach (var obj in ta)
			{
				Console.Write(obj.get_str() + obj.get() + " ");
			}
			Console.WriteLine();
			Console.WriteLine("len=" + ta.Count);
		}

		private static void test3()
		{
			int[,,] arr3 = new int[12, 23, 34];
			Console.WriteLine(arr3.GetLength(0));
			Console.WriteLine(arr3.GetLength(1));
			Console.WriteLine(arr3.GetLength(2));
		}

		private static void test4()
		{
			//string jStr = "[{\"name\":\"0\",\"weight\":[0,0,0.1111111111111111,0.54563492063492069,0.54563492063492069,0.54563492063492069,0.2361111111111111,0.013888888888888895,0,0,0,0,0.72658730158730167,0.88888888888888884,0.45436507936507936,0.74007936507936511,0.98611111111111116,0.70436507936507931,0,0,0,0.45436507936507936,1,0.88888888888888884,0.0043650793650794606,0.0043650793650794606,0.62341269841269842,0.47658730158730167,0.45436507936507936,0.25436507936507946,0.45436507936507936,0.45436507936507936,0.59563492063492052,0,0,0,0.073412698412698429,0.30992063492063482,0.23769841269841263,0.45436507936507936,0.45436507936507936,0,0.37896825396825395,0,0,0,0.073412698412698429,0.2361111111111111,0,0.45436507936507936,0.45436507936507936,0.0043650793650794606,0.37896825396825395,0,0,0,0.073412698412698429,0.37896825396825395,0,0.45436507936507936,0.23769841269841263,0.45436507936507936,0.59563492063492052,0.62341269841269842,0.0043650793650794606,0,0.17103174603174598,0.59563492063492052,0,0.45436507936507936,0,0.45436507936507936,0.92658730158730163,0.88888888888888884,0.0043650793650794606,0.25436507936507946,0.58769841269841261,1,0.25436507936507946,0.071031746031746112,0,0,0.1111111111111111,0.4876984126984128,0.52341269841269833,0.12658730158730158,0.45992063492063495,0.19007936507936513,0,0,0,0,0.1111111111111111,0.2361111111111111,0.54563492063492069,0.54563492063492069,0.32341269841269843,0,0,0],\"countTrainig\":9}]";
			string jStr = "[{\"name\":\"0\",\"weight\":[0.1,0.2,0.3],\"count\":9}]";
			JavaScriptSerializer json = new JavaScriptSerializer();
			List<Object> objects = json.Deserialize<List<Object>>(jStr);
			System.Console.WriteLine("objects.count = " + objects.Count);
			foreach (var o in objects)
			{
				Dictionary<string, object> temp = (Dictionary<string, Object>)o;
				string name = (string)temp["name"];
				int count = (int)temp["count"];

				/*
				Object[] weightData = ((ArrayList)temp["weight"]).ToArray();
				foreach (Object w in weightData)
				{
					System.Console.WriteLine(w.ToString());
				}
				*/

				IEnumerable weightData = (IEnumerable)temp["weight"];
				int i = 0;
				foreach (Object weight in weightData)
				{
					System.Console.WriteLine($"weight[{i}] = {weight}");
					i++;
				}

				System.Console.WriteLine("name = " + name);
				System.Console.WriteLine("count = " + count);
			}
		}

		private static void test5(ref int value)
		{
			value = 5;
		}

		private static void test6(int[,] arr)
		{
			for (int y = 0; y < arr.GetLength(0); y++)
			{
				for (int x = 0; x < arr.GetLength(1); x++)
				{
					int res = arr[x, y];
					if (res != 0)
					{
						System.Console.WriteLine("x=" + x + " y=" + y + " res=" + res);
					}
				}
			}
		}

		public static void Main(string[] args)
		{
			//test();
			//test2();
			//test3();
			test4();

			/*
			int a = 0;
			test5(ref a);
			System.Console.WriteLine("a = " + a);
			*/

			/*
			int[,] arr = new int[10000,10000];
			for (int y = 0; y < arr.GetLength(0); y++)
			{
				for (int x = 0; x < arr.GetLength(1); x++)
				{
					arr[x,y] = 0;
				}
			}
			arr[5, 5] = 6;
			test6(arr);
			*/

			MessageBox.Show("test");
		}
	}
	//------------------------------------------------------
}
