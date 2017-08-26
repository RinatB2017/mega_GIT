using System;
using System.Drawing;
using AForge;
using AForge.Imaging.Filters;

using Point = System.Drawing.Point;

namespace PlayingCardRecognition
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
        private Point[] corners ;//Corner points of card on source image

        //Properties
        public Point[] Corners
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
            corners = new Point[total]; 

            for(int i = 0 ; i < total ; i++)
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
            //Crop crop = new Crop(new Rectangle(image.Width - 37, 10, 30, 60));
            Crop crop = new Crop(new Rectangle(0, 0, 32, 90));

            return crop.Apply(image);
        }

        /// <summary>
        /// Function that crops top-right part of card image , with size 30x60
        /// </summary>
        /// <returns>Cropped image</returns>
        public Bitmap GetTopLeftRankPart()
        {
            if (image == null)
                return null;

            Crop crop = new Crop(new Rectangle(0, 4, 40, 43));

            return crop.Apply(image);
        }

        /// <summary>
        /// Function that crops top-right part of card image , with size 30x60
        /// </summary>
        /// <returns>Cropped image</returns>
        public Bitmap GetTopLeftSuitPart()
        {
            if (image == null)
                return null;

            Crop crop = new Crop(new Rectangle(0, 49, 40, 45));

            return crop.Apply(image);
        }

        /// <summary>
        /// Overrided ToString Function.
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            if (suit == Suit.NOT_RECOGNIZED ||
                rank == Rank.NOT_RECOGNIZED)
                return "Processando...";

            string suitStr = string.Empty;
            string rankStr = string.Empty;

            //Convert suit value to string 
            switch (suit)
            {
                case Suit.Clubs:
                    suitStr = "Paus";
                    break;
                case Suit.Diamonds:
                    suitStr = "Ouros";
                    break;
                case Suit.Hearts:
                    suitStr = "Copas";
                    break;
                case Suit.Spades:
                    suitStr = "Espadas";
                    break;
            }
            //Convert Rank Value To String
            switch (rank)
            {
                case Rank.Ace:
                    rankStr = "Ás";
                    break;
                case Rank.Two:
                    rankStr = "Dois";
                    break;
                case Rank.Three:
                    rankStr = "Três";
                    break;
                case Rank.Four:
                    rankStr = "Quatro";
                    break;
                case Rank.Five:
                    rankStr = "Cinco";
                    break;
                case Rank.Six:
                    rankStr = "Seis";
                    break;
                case Rank.Seven:
                    rankStr = "Sete";
                    break;
                case Rank.Eight:
                    rankStr = "Oito";
                    break;
                case Rank.Nine:
                    rankStr = "Nove";
                    break;
                case Rank.Ten:
                    rankStr = "Dez";
                    break;
                case Rank.Jack:
                    rankStr = "Valete";
                    break;
                case Rank.Queen:
                    rankStr = "Dama";
                    break;
                case Rank.King:
                    rankStr = "Rei";
                    break;
            }
            return rankStr + " de " + suitStr;
        }
    }
}
