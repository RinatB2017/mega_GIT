﻿using System;
using System.Collections.Generic;

using System.Text;

using HeatonResearchNeural.Matrix;
using HeatonResearchNeural.Util;

namespace HeatonResearchNeural.Feedforward.Train.Backpropagation
{
    public class BackpropagationLayer
    {
        /// <summary>
        /// Errors for this layer.
        /// </summary>
        private double[] error;

        /// <summary>
        /// The error deltas for this layer.
        /// </summary>
        private double[] errorDelta;

        /// <summary>
        /// Accumulate the error deltas for each weight matrix and bias value.
        /// </summary>
        private Matrix.Matrix accMatrixDelta;

        /// <summary>
        /// The bias values are stored in a "virtual row" just beyond the regular
        /// weight rows. This variable holds the index to that location.
        /// </summary>
        private int biasRow;

        /// <summary>
        /// Hold the previous matrix deltas so that "momentum" can be implemented.
        /// This handles both weights and thresholds.
        /// </summary>
        private Matrix.Matrix matrixDelta;

        /// <summary>
        /// The parent object.
        /// </summary>
        private Backpropagation backpropagation;

        /// <summary>
        /// The actual layer that this training layer corresponds to.
        /// </summary>
        private FeedforwardLayer layer;

        /// <summary>
        /// Construct a BackpropagationLayer object that corresponds to a specific neuron layer.
        /// </summary>
        /// <param name="backpropagation">The back propagation training object.</param>
        /// <param name="layer">The layer that this object corresponds to.</param>
        public BackpropagationLayer(Backpropagation backpropagation,
                 FeedforwardLayer layer)
        {
            this.backpropagation = backpropagation;
            this.layer = layer;

            int neuronCount = layer.NeuronCount;

            this.error = new double[neuronCount];
            this.errorDelta = new double[neuronCount];

            if (layer.Next != null)
            {
                this.accMatrixDelta = new Matrix.Matrix(layer.NeuronCount + 1, layer.Next.NeuronCount);
                this.matrixDelta = new Matrix.Matrix(layer.NeuronCount + 1, layer.Next.NeuronCount);
                this.biasRow = layer.NeuronCount;
            }
        }

        /// <summary>
        /// Accumulate a matrix delta.
        /// </summary>
        /// <param name="i1">The matrix row.</param>
        /// <param name="i2">The matrix column.</param>
        /// <param name="value">The delta value.</param>
        public void AccumulateMatrixDelta(int i1, int i2,
                 double value)
        {
            this.accMatrixDelta.Add(i1, i2, value);
        }

        /// <summary>
        /// Accumulate a threshold delta.
        /// </summary>
        /// <param name="index">The threshold index.</param>
        /// <param name="value">The threshold value.</param>
        public void AccumulateThresholdDelta(int index, double value)
        {
            this.accMatrixDelta.Add(this.biasRow, index, value);
        }

        /// <summary>
        /// Calculate the current error.
        /// </summary>
        public void CalcError()
        {

            BackpropagationLayer next = this.backpropagation
                   .GetBackpropagationLayer(this.layer.Next);

            for (int i = 0; i < this.layer.Next.NeuronCount; i++)
            {
                for (int j = 0; j < this.layer.NeuronCount; j++)
                {
                    AccumulateMatrixDelta(j, i, next.GetErrorDelta(i)
                            * this.layer.GetFire(j));
                    SetError(j, GetError(j) + this.layer.LayerMatrix[j, i]
                            * next.GetErrorDelta(i));
                }
                AccumulateThresholdDelta(i, next.GetErrorDelta(i));
            }

            if (this.layer.IsHidden())
            {
                // hidden layer deltas
                for (int i = 0; i < this.layer.NeuronCount; i++)
                {
                    SetErrorDelta(i, BoundNumbers.Bound(CalculateDelta(i)));
                }
            }

        }

        /// <summary>
        /// Calculate the error for the given ideal values.
        /// </summary>
        /// <param name="ideal">Ideal output values.</param>
        public void CalcError(double[] ideal)
        {

            // layer errors and deltas for output layer
            for (int i = 0; i < this.layer.NeuronCount; i++)
            {
                SetError(i, ideal[i] - this.layer.GetFire(i));
                SetErrorDelta(i, BoundNumbers.Bound(CalculateDelta(i)));
            }
        }


        /// <summary>
        /// Calculate the delta for actual vs ideal. This is the amount that will be
        /// applied during learning.
        /// </summary>
        /// <param name="i">The neuron being calculated for.</param>
        /// <returns>The delta to be used to learn.</returns>
        private double CalculateDelta(int i)
        {
            return GetError(i) *
            this.layer.LayerActivationFunction.DerivativeFunction(this.layer.GetFire(i));
        }

        /// <summary>
        /// Clear any error values.
        /// </summary>
        public void ClearError()
        {
            for (int i = 0; i < this.layer.NeuronCount; i++)
            {
                this.error[i] = 0;
            }
        }


        /// <summary>
        /// Get the error for the specified neuron.
        /// </summary>
        /// <param name="index">The index for the specified neuron.</param>
        /// <returns>The error for the specified neuron.</returns>
        public double GetError(int index)
        {
            return this.error[index];
        }

        /// <summary>
        /// Get the error delta for the specified neuron.
        /// </summary>
        /// <param name="index">The specified neuron.</param>
        /// <returns>The error delta.</returns>
        public double GetErrorDelta(int index)
        {
            return this.errorDelta[index];
        }


        /// <summary>
        /// Learn from the last error calculation.
        /// </summary>
        /// <param name="learnRate">The learning rate.</param>
        /// <param name="momentum">The momentum.</param>
        public void Learn(double learnRate, double momentum)
        {
            // process the matrix
            if (this.layer.HasMatrix())
            {

                Matrix.Matrix m1 = MatrixMath.Multiply(this.accMatrixDelta,
                       learnRate);
                Matrix.Matrix m2 = MatrixMath.Multiply(this.matrixDelta, momentum);
                this.matrixDelta = MatrixMath.Add(m1, m2);
                this.layer.LayerMatrix = (MatrixMath.Add(this.layer.LayerMatrix,
                        this.matrixDelta));
                this.accMatrixDelta.Clear();
            }
        }


        /// <summary>
        /// Set the error for the specified neuron.
        /// </summary>
        /// <param name="index">The specified neuron.</param>
        /// <param name="e">The error value.</param>
        public void SetError(int index, double e)
        {
            this.error[index] = BoundNumbers.Bound(e);
        }

        /// <summary>
        /// Set the error delta for the specified neuron.
        /// </summary>
        /// <param name="index">The specified neuron.</param>
        /// <param name="d">The error delta.</param>
        public void SetErrorDelta(int index, double d)
        {
            this.errorDelta[index] = d;
        }
    }
}
