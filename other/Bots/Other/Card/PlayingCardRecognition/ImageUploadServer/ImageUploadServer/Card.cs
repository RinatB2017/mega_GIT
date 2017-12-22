using AForge;
using AForge.Imaging.Filters;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Web;

namespace ImageUploadServer
{
    /// <summary>
    /// Rank enumeration
    /// </summary>
    public enum Rank
    {
        NOT_RECOGNIZED = 0,
        Ace = 1,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        Ten,
        Jack,
        Queen,
        King
    }
    /// <summary>
    /// Suit enumeration
    /// </summary>
    public enum Suit
    {
        NOT_RECOGNIZED = 0,
        Hearts,
        Diamonds,
        Spades,
        Clubs
    }
    /// <summary>
    /// Class represents recognized playing card. Contains properties of card such as ; 
    /// Rank of card, suit of card, image of card which is extracted from source image ,
    /// corner points of card on source image
    /// </summary>
    public class Card
    {
        //Variables
        private Rank rank; //Rank of card
        private Suit suit; //Suit of card
        private Bitmap image; //Extracted(transformed) image of card
        private System.Drawing.Point[] corners;//Corner points of card on source image

        //Properties
        public System.Drawing.Point[] Corners
        {
            get { return this.corners; }
        }
        public Rank Rank
        {
            set { this.rank = value; }
        }
        public Suit Suit
        {
            set { this.suit = value; }
        }
        public Bitmap Image
        {
            get { return this.image; }
        }
        //Constructor
        public Card(Bitmap cardImg, IntPoint[] cornerIntPoints)
        {
            this.image = cardImg;

            //Convert AForge.IntPoint Array to System.Drawing.Point Array
            int total = cornerIntPoints.Length;
            corners = new System.Drawing.Point[total];

            for (int i = 0; i < total; i++)
            {
                this.corners[i].X = cornerIntPoints[i].X;
                this.corners[i].Y = cornerIntPoints[i].Y;
            }
        }
        /// <summary>
        /// Function that crops top-right part of card image , with size 30x60
        /// </summary>
        /// <returns>Cropped image</returns>
        public Bitmap GetTopLeftPart()
        {
            if (image == null)
                return null;
            Crop crop = new Crop(new Rectangle(0, 0, 30, 84));


            Bitmap im2 = crop.Apply(image);
            //im2.Save("cropped", System.Drawing.Imaging.ImageFormat.Jpeg);
            return crop.Apply(image);
        }
        /// <summary>
        /// Overrided ToString Function.
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            if (suit == ImageUploadServer.Suit.NOT_RECOGNIZED ||
                rank == ImageUploadServer.Rank.NOT_RECOGNIZED)
                return string.Empty;

            string suitStr = string.Empty;
            string rankStr = string.Empty;

            //Convert suit value to string 
            switch (suit)
            {
                case Suit.Clubs:
                    suitStr = "Clubs";
                    break;
                case Suit.Diamonds:
                    suitStr = "Diamonds";
                    break;
                case Suit.Hearts:
                    suitStr = "Hearts";
                    break;
                case Suit.Spades:
                    suitStr = "Spades";
                    break;
            }
            //Convert Rank Value To String
            switch (rank)
            {
                case Rank.Ace:
                    rankStr = "Ace";
                    break;
                case Rank.Two:
                    rankStr = "Two";
                    break;
                case Rank.Three:
                    rankStr = "Three";
                    break;
                case Rank.Four:
                    rankStr = "Four";
                    break;
                case Rank.Five:
                    rankStr = "Five";
                    break;
                case Rank.Six:
                    rankStr = "Six";
                    break;
                case Rank.Seven:
                    rankStr = "Seven";
                    break;
                case Rank.Eight:
                    rankStr = "Eight";
                    break;
                case Rank.Nine:
                    rankStr = "Nine";
                    break;
                case Rank.Ten:
                    rankStr = "Ten";
                    break;
                case Rank.Jack:
                    rankStr = "Jack";
                    break;
                case Rank.Queen:
                    rankStr = "Queen";
                    break;
                case Rank.King:
                    rankStr = "King";
                    break;
            }
            return rankStr + " of " + suitStr;
        }
    }
}