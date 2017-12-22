﻿using System;
using System.Collections.Generic;
using System.Text;

using HeatonResearchNeural.Anneal;
using HeatonResearchNeural.Matrix;

namespace HeatonResearchNeural.Feedforward.Train.Anneal
{
    public class NeuralSimulatedAnnealing : SimulatedAnnealing<double>,Train
    {
        /// <summary>
        /// Get the best network from the training.
        /// </summary>
        public FeedforwardNetwork Network
        {
            get
            {
                return this.network;
            }
        }

        /// <summary>
        /// The neural network that is to be trained.
        /// </summary>
        protected FeedforwardNetwork network;

        /// <summary>
        /// The training data.
        /// </summary>
        protected double[][] input;

        /// <summary>
        /// The ideal results to the training data.
        /// </summary>
        protected double[][] ideal;

        /// <summary>
        /// Construct a simulated annleaing trainer for a feedforward neural network.
        /// </summary>
        /// <param name="network">The neural network to be trained.</param>
        /// <param name="input">The input values for training.</param>
        /// <param name="ideal">The ideal values for training.</param>
        /// <param name="startTemp">The starting temperature.</param>
        /// <param name="stopTemp">The ending temperature.</param>
        /// <param name="cycles">The number of cycles in a training iteration.</param>
        public NeuralSimulatedAnnealing(FeedforwardNetwork network,
                 double[][] input, double[][] ideal,
                 double startTemp, double stopTemp, int cycles)
        {
            this.network = network;
            this.input = input;
            this.ideal = ideal;
            this.temperature = startTemp;
            this.StartTemperature = startTemp;
            this.StopTemperature = stopTemp;
            this.Cycles = cycles;
        }


        /// <summary>
        /// Determine the error of the current weights and thresholds.
        /// </summary>
        /// <returns>The error.</returns>
        override public double DetermineError()
        {
            return this.network.CalculateError(this.input, this.ideal);
        }

        /// <summary>
        /// Get the network as an array of doubles.
        /// </summary>
        /// <returns>The network as an array of doubles.</returns>
        override public double[] GetArray()
        {
            return MatrixCODEC.NetworkToArray(this.network);
        }

        /// <summary>
        /// Convert an array of doubles to the current best network.
        /// </summary>
        /// <param name="array">The array that contains the neural network.</param>
        override public void PutArray(double[] array)
        {
            MatrixCODEC.ArrayToNetwork(array, this.network);
        }

        /// <summary>
        /// Randomize the weights and thresholds. This function does most of the work
        /// of the class. Each call to this class will randomize the data according
        /// to the current temperature. The higher the temperature the more
        /// randomness.
        /// </summary>
        override public void Randomize()
        {
            Random rand = new Random();
            double[] array = MatrixCODEC.NetworkToArray(this.network);

            for (int i = 0; i < array.Length; i++)
            {
                double add = 0.5 - (rand.NextDouble());
                add /= this.StartTemperature;
                add *= this.temperature;
                array[i] = array[i] + add;
            }

            MatrixCODEC.ArrayToNetwork(array, this.network);
        }

        /// <summary>
        /// Get the network as an array of doubles.
        /// </summary>
        /// <returns>The network as an array of doubles.</returns>
        override public double[] GetArrayCopy()
        {
            return this.GetArray();
        }
    }
}
