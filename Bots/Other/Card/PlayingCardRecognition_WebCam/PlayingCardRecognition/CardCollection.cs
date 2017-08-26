using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;

namespace PlayingCardRecognition
{
    /// <summary>
    /// A Collection class that holds card objects 
    /// Derived from CollectionBase class
    /// </summary>
    public class CardCollection : CollectionBase
    {
        public void Add(Card card)
        {
            this.List.Add(card);
        }
        public Card this[int index]
        {
            get { return this.List[index] as Card; }
            set { this.List[index] = value; }
        }
        /// <summary>
        /// Function that returns list of images of carrds in collection. 
        /// </summary>
        /// <returns>Images of Cards</returns>
        public List<Bitmap> ToImageList()
        {
            List<Bitmap> list = new List<Bitmap>();

            foreach (Card card in this.List)
                list.Add(card.Image);

            return list;
        }
    }
}
