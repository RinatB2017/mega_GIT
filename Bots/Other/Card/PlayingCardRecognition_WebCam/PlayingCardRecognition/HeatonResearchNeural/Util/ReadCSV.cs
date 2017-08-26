﻿using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using System.Globalization;

namespace HeatonResearchNeural.Util
{
    public class ReadCSV
    {
        /// <summary>
        /// The file to read.
        /// </summary>
        private TextReader reader;

        /// <summary>
        /// The names of all of the columns, read from the first line of the file.
        /// </summary>
        private IDictionary<String, int> columns = new Dictionary<String, int>();

        /// <summary>
        /// The data for the current line.
        /// </summary>
        private String[] data;


        /// <summary>
        /// Format a date/time object to the same format that we parse in.
        /// </summary>
        /// <param name="date">The date to format.</param>
        /// <returns>A formatted date and time.</returns>
        public static String DisplayDate(DateTime date)
        {
            return date.ToString();
        }

        /// <summary>
        /// Parse a date using the specified format.
        /// </summary>
        /// <param name="when">A string that contains a date in the specified format.</param>
        /// <returns>A DateTime that was parsed.</returns>
        public static DateTime ParseDate(String when)
        {
            try
            {
                return DateTime.ParseExact(when, "yyyy-MM-dd",CultureInfo.InvariantCulture);
            }
            catch (FormatException)
            {
                return default(DateTime);
            }
        }

        /// <summary>
        /// Construct an object to read the specified CSV file.
        /// </summary>
        /// <param name="filename">The filename to read.</param>
        public ReadCSV(String filename)
        {
            this.reader = new StreamReader(filename);

            // read the column heads
            String line = this.reader.ReadLine();
            string[] tok = line.Split(',');

            for (int index = 0; index < tok.Length; index++)
            {
                String header = tok[index];
                this.columns.Add(header.ToLower(), index);
            }

            this.data = new String[tok.Length];
        }

        /// <summary>
        /// Close the file.
        /// </summary>
        public void Close()
        {
            this.reader.Close();
        }

        /// <summary>
        /// Get the specified column using an index.
        /// </summary>
        /// <param name="i">The zero based index of the column to read.</param>
        /// <returns>The specified column as a string.</returns>
        public String Get(int i)
        {
            return this.data[i];
        }

        /// <summary>
        /// Get the specified column as a string.
        /// </summary>
        /// <param name="column">The specified column.</param>
        /// <returns>The specified column as a string.</returns>
        public String Get(String column)
        {
            if (!columns.ContainsKey(column.ToLower()))
            {
                return null;
            }
            int i = this.columns[column.ToLower()];

            return this.data[i];
        }

        /// <summary>
        /// Read the specified column as a date.
        /// </summary>
        /// <param name="column">The specified column.</param>
        /// <returns>The specified column as a DateTime.</returns>
        public DateTime GetDate(String column)
        {
            String str = Get(column);
            return DateTime.Parse(str);
        }

        /// <summary>
        /// Get the specified column as a double.
        /// </summary>
        /// <param name="column">The column to read.</param>
        /// <returns>The specified column as a double.</returns>
        public double GetDouble(String column)
        {
            String str = Get(column);
            return double.Parse(str);
        }

        /// <summary>
        /// Get an integer that has the specified name.
        /// </summary>
        /// <param name="col">The column name to read.</param>
        /// <returns>The specified column as an int.</returns>
        public int GetInt(String col)
        {
            String str = Get(col);
            try
            {
                return int.Parse(str);
            }
            catch (FormatException)
            {
                return 0;
            }
        }

        /// <summary>
        /// Read the next line.
        /// </summary>
        /// <returns>Return false if there are no more lines in the file.</returns>
        public bool Next()
        {
            String line = this.reader.ReadLine();
            if (line == null)
            {
                return false;
            }

            string[] tok = line.Split(',');

            for (int i = 0; i < tok.Length; i++)
            {
                String str = tok[i];
                if (i < this.data.Length)
                {
                    this.data[i] = str;
                }
            }

            return true;
        }
    }
}
