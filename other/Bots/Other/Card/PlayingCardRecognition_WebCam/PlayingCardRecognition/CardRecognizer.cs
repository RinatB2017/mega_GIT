using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.Serialization.Formatters.Binary;
using AForge;
using AForge.Imaging.Filters;
using AForge.Imaging;

using System.IO;
using AForge.Math.Geometry;

    //Remove ambiguousness between AForge.Image and System.Drawing.Image
using HeatonResearchNeural.Feedforward;
using Point = System.Drawing.Point; //Remove ambiguousness between AForge.Point and System.Drawing.Point

namespace PlayingCardRecognition
{
    /// <summary>
    /// Class that recognizes playing cards from image
    /// 
    /// </summary>
    public class CardRecognizer
    {
        private FeedforwardNetwork NetworkLetra;
        private FeedforwardNetwork NetworkSuits;
        private FeedforwardNetwork NetworkNumero;

        private const int CardWidth = 200;  //Card width for scaling
        private const int CardHeight = 300; //Card height for scaling

        //Template Images
        private Bitmap j, k, q; //Face Card Character Templates
        private Bitmap clubs, diamonds, spades, hearts; //Suit Templates

        private FiltersSequence commonSeq; //Commonly filter sequence to be used

        static Assembly CurrentDomain_AssemblyResolve(object sender, ResolveEventArgs args)
        {
            try
            {
                if (args.Name == "AproxFunction")
                {
                    //Load my Assembly 
                    Assembly assem = Assembly.LoadFrom("AproxFunction");
                    if (assem != null)
                        return assem;
                }
            }
            catch { ;}

            return Assembly.GetExecutingAssembly();
        }

        public CardRecognizer()
        {
            //Initialize common filter sequence , this sequence generally will be applied
            commonSeq = new FiltersSequence();
            commonSeq.Add(Grayscale.CommonAlgorithms.BT709);
            commonSeq.Add(new OtsuThreshold());
            commonSeq.Add(new DifferenceEdgeDetector());
            
            Stream strm;
            BinaryFormatter bformat;

            AppDomain.CurrentDomain.AssemblyResolve += CurrentDomain_AssemblyResolve;

            strm = File.Open("NetworkLetra.net", FileMode.Open);
            bformat = new BinaryFormatter();

            NetworkLetra = (FeedforwardNetwork)bformat.Deserialize(strm);

            strm = File.Open("NetworkSuits.net", FileMode.Open);
            bformat = new BinaryFormatter();

            NetworkSuits = (FeedforwardNetwork) bformat.Deserialize(strm);

            strm = File.Open("NetworkNumero.net", FileMode.Open);
            bformat = new BinaryFormatter();

            NetworkNumero = (FeedforwardNetwork)bformat.Deserialize(strm);

            AppDomain.CurrentDomain.AssemblyResolve -= CurrentDomain_AssemblyResolve;
        }

        private Suit ScanSuit(Bitmap bmp)
        {
            int height = bmp.Height;
            int width = bmp.Width;
            var lista = new List<double>();

            for (int x = 0; x < width; x++)
            {
                for (int y = 0; y < height; y++)
                {
                    var c = bmp.GetPixel(x, y);
                    if (c.R < 200) lista.Add(1.0);
                    else lista.Add(0.0);
                }
            }

            double[] saida = NetworkSuits.ComputeOutputs(lista.ToArray());

            double comp = 0.0;
            int pos = 0;
            for (int i = 0; i < saida.Length; i++)
            {
                if (saida[i] > 0.3 && saida[i] > comp)
                {
                    comp = saida[i];
                    pos = i + 1;
                }
            }

            if (pos == 1) return Suit.Hearts;
            if (pos == 2) return Suit.Spades;
            if (pos == 3) return Suit.Diamonds;
            if (pos == 4) return Suit.Clubs;
            return Suit.NOT_RECOGNIZED;
        }

        /// <summary>
        /// Scans rank of face cards
        /// </summary>
        /// <param name="cardImage"></param>
        /// <returns></returns>
        private Rank ScanFaceRank(Bitmap bmp)
        {
            int height = bmp.Height;
            int width = bmp.Width;
            var lista = new List<double>();

            for (int x = 0; x < width; x++)
            {
                for (int y = 0; y < height; y++)
                {
                    var c = bmp.GetPixel(x, y);
                    if (c.R < 200) lista.Add(1.0);
                    else lista.Add(0.0);
                }
            }

            double comp = 0.0;
            int pos = 0;
            double[] saida = NetworkLetra.ComputeOutputs(lista.ToArray());
            for (int i = 0; i < saida.Length; i++)
            {
                if (saida[i] > 0.3 && saida[i] > comp)
                {
                    comp = saida[i];
                    pos = i + 1;
                }
            }

            if (pos == 1) return Rank.Ace;
            if (pos == 2) return Rank.Jack;
            if (pos == 3) return Rank.King;
            if (pos == 4) return Rank.Queen;
            return Rank.NOT_RECOGNIZED;
        }

        /// <summary>
        /// Scans rank of card and returns result.
        /// For recognizing rank, counts suits on image 
        /// </summary>
        /// <param name="cardImage">Card image to be scanned</param>
        /// <returns>Rank of card</returns>
        private Rank ScanRank(Bitmap bmp)
        {
            int height = bmp.Height;
            int width = bmp.Width;
            var lista = new List<double>();

            for (int x = 0; x < width; x++)
            {
                for (int y = 0; y < height; y++)
                {
                    var c = bmp.GetPixel(x, y);
                    if (c.R < 200) lista.Add(1.0);
                    else lista.Add(0.0);
                }
            }

            double comp = 0.0;
            int pos = 0;
            double[] saida = NetworkNumero.ComputeOutputs(lista.ToArray());
            for (int i = 0; i < saida.Length; i++)
            {
                if (saida[i] > 0.5 && saida[i] > comp)
                {
                    comp = saida[i];
                    pos = i + 1;
                }
            }

            if (comp > 0.5)
            {
                if (pos == 1) return Rank.Two;
                if (pos == 2) return Rank.Three;
                if (pos == 3) return Rank.Four;
                if (pos == 4) return Rank.Five;
                if (pos == 5) return Rank.Six;
                if (pos == 6) return Rank.Seven;
                if (pos == 7) return Rank.Eight;
                if (pos == 8) return Rank.Nine;
                if (pos == 9) return Rank.Ten;
                if (pos == 10) return Rank.Ace;
            }
            return Rank.NOT_RECOGNIZED;
        }

        /// <summary>
        /// Determines whether card is a face card(Jack,Queen,King) or not.
        /// If card is a face card , then it will have  a big blob whose width will be
        /// larger than half width of card
        /// If card isn't a face card, then width of all blobs will be less than half width of card
        /// </summary>
        /// <param name="bmp">Card of image to be analyzed</param>
        /// <returns>True if its a face card, false if not</returns>
        private bool IsFaceCard(Bitmap bmp)
        {
            Bitmap temp = this.commonSeq.Apply(bmp);
            ExtractBiggestBlob extractor = new ExtractBiggestBlob();
            temp = extractor.Apply(temp); //Extract biggest blob
            if (temp.Height > bmp.Height / 5)  //If width is larger than half width of card
                return true; //Its a face card

            return false;  //It is not a face card
        }

        /// <summary>
        /// Returns point who has minimum x and y
        /// </summary>
        /// <param name="points">Points to be searched</param>
        /// <returns>Returns point who has minimum x and y</returns>
        public static Point GetStringPoint(Point[] points)
        {
            Point[] tempArr = new Point[points.Length];
            Array.Copy(points, tempArr, points.Length);
            Array.Sort(tempArr, new PointComparer());

            return tempArr[0].X < tempArr[1].X ? tempArr[0] : tempArr[1];
        }
        /// <summary>
        /// Detects and recognizes cards from source image
        /// </summary>
        /// <param name="source">Source image to be scanned</param>
        /// <returns>Recognized Cards</returns>
        public CardCollection Recognize(Bitmap source)
        {
            CardCollection collection = new CardCollection();  //Collection that will hold cards
            Bitmap temp = source.Clone() as Bitmap; //Clone image to keep original image

            FiltersSequence seq = new FiltersSequence();
            seq.Add(Grayscale.CommonAlgorithms.BT709);  //First add  grayScaling filter
            seq.Add(new OtsuThreshold()); //Then add binarization(thresholding) filter
            temp = seq.Apply(source); // Apply filters on source image

            //Extract blobs from image whose size width and height larger than 150
            BlobCounter extractor = new BlobCounter();
            extractor.FilterBlobs = true;
            extractor.MinWidth = extractor.MinHeight = 150;
            extractor.MaxWidth = extractor.MaxHeight = 350;
            extractor.ProcessImage(temp);

            //Will be used transform(extract) cards on source image 
            QuadrilateralTransformation quadTransformer = new QuadrilateralTransformation();

            //Will be used resize(scaling) cards 
            ResizeBilinear resizer = new ResizeBilinear(CardWidth, CardHeight);

            foreach (Blob blob in extractor.GetObjectsInformation())
            {
                //Get Edge points of card
                List<IntPoint> edgePoints = extractor.GetBlobsEdgePoints(blob);
                //Calculate/Find corners of card on source image from edge points
                List<IntPoint> corners = PointsCloud.FindQuadrilateralCorners(edgePoints);

                quadTransformer.SourceQuadrilateral = corners; //Set corners for transforming card 
                quadTransformer.AutomaticSizeCalculaton = true;

                Bitmap cardImg = quadTransformer.Apply(source); //Extract(transform) card image

                if (cardImg.Width > cardImg.Height) //If card is positioned horizontally
                    cardImg.RotateFlip(RotateFlipType.Rotate90FlipNone); //Rotate
                cardImg = resizer.Apply(cardImg); //Normalize card size

                Card card = new Card(cardImg, corners.ToArray()); //Create Card Object
                bool faceCard = IsFaceCard(cardImg); //Determine type of card(face or not)

                ResizeBicubic res;

                seq.Clear();
                seq.Add(Grayscale.CommonAlgorithms.BT709);
                seq.Add(new OtsuThreshold());

                Bitmap topLeftSuit = card.GetTopLeftSuitPart();
                Bitmap bmp = seq.Apply(topLeftSuit);

                bmp = CutWhiteSpaces(bmp);
                res = new ResizeBicubic(32, 40);
                bmp = res.Apply(bmp);

                Bitmap topLeftRank = card.GetTopLeftRankPart();
                Bitmap bmp2 = seq.Apply(topLeftRank);

                bmp2 = CutWhiteSpaces(bmp2);


                seq.Clear();
                seq.Add(new OtsuThreshold());
                bmp = seq.Apply(bmp);
                card.Suit = ScanSuit(bmp);

                if (!faceCard)
                {
                    res = new ResizeBicubic(26, 40);
                    bmp2 = res.Apply(bmp2);
                    seq.Clear();
                    seq.Add(new OtsuThreshold());
                    bmp2 = seq.Apply(bmp2);
                    card.Rank = ScanRank(bmp2);
                }
                else
                {
                    res = new ResizeBicubic(32, 40);
                    bmp2 = res.Apply(bmp2);
                    seq.Clear();
                    seq.Add(new OtsuThreshold());
                    bmp2 = seq.Apply(bmp2);
                    card.Rank = ScanFaceRank(bmp2);
                }
                collection.Add(card); //Add card to collection
            }
            return collection;
        }

        public Bitmap CutWhiteSpaces(Bitmap bmp)
        {
            Bitmap tempBlob = bmp;

            int x, y;

            x = 0;
            y = 0;
            while (true)
            {
                if (tempBlob.GetPixel(x, y).R < 127) break;
                y++;
                if (y >= tempBlob.Height)
                {
                    y = 0;
                    x++;
                    if (x >= tempBlob.Width) break;
                }
            }
            int xMin = x;

            x = tempBlob.Width - 1;
            y = 0;
            while (true)
            {
                if (tempBlob.GetPixel(x, y).R < 127) break;
                y++;
                if (y >= tempBlob.Height)
                {
                    y = 0;
                    x--;
                    if (x == 0) break;
                }
            }
            int xMax = x;

            x = 0;
            y = 0;
            while (true)
            {
                if (tempBlob.GetPixel(x, y).R < 127) break;
                x++;
                if (x >= tempBlob.Width)
                {
                    x = 0;
                    y++;
                    if (y >= tempBlob.Height) break;
                }
            }
            int yMin = y;

            x = 0;
            y = tempBlob.Height - 1;
            while (true)
            {
                if (tempBlob.GetPixel(x, y).R < 127) break;
                x++;
                if (x >= tempBlob.Width)
                {
                    x = 0;
                    y--;
                    if (y >= tempBlob.Height) break;
                }
            }
            int yMax = y;

            Crop crop = new Crop(new Rectangle(xMin, yMin, xMax - xMin+1, yMax - yMin+1));
            tempBlob = crop.Apply(bmp);

            return tempBlob;
        }
    }
}
