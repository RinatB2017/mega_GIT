﻿using System;
using System.Threading;
using System.Collections.Generic;
using System.Text;

namespace HeatonResearchNeural.Genetic
{
    public abstract class GeneticAlgorithm<GENE_TYPE>
    {
        /// <summary>
        /// The current best error for the population.
        /// </summary>
        public double Error
        {
            get
            {
                return this.chromosomes[0].Cost;
            }
        }

        /// <summary>
        /// How many chromosomes should be used.
        /// </summary>
        public int PopulationSize
        {
            get
            {
                return this.populationSize;
            }
            set
            {
                this.populationSize = value;
            }
        }

        /// <summary>
        /// What percent of "births" should result in a mutation.
        /// </summary>
        public double MutationPercent
        {
            get
            {
                return this.mutationPercent;
            }
            set
            {
                this.mutationPercent = value;
            }
        }

        /// <summary>
        /// What percent should be allowed to mate.
        /// </summary>
        public double PercentToMate
        {
            get
            {
                return this.percentToMate;
            }
            set
            {
                this.percentToMate = value;
            }
        }

        /// <summary>
        /// What is the size of the mating population.
        /// </summary>
        public double MatingPopulation
        {
            get
            {
                return this.matingPopulation;
            }
            set
            {
                this.matingPopulation = value;
            }
        }

        /// <summary>
        /// Should repeats in the genetic material be prevented.
        /// </summary>
        public bool PreventRepeat
        {
            get
            {
                return this.preventRepeat;
            }
            set
            {
                this.preventRepeat = value;
            }
        }

        /// <summary>
        /// What size cut should be used for each mating.
        /// </summary>
        public int CutLength
        {
            get
            {
                return this.cutLength;
            }
            set
            {
                this.cutLength = value;
            }
        }

        /// <summary>
        /// Should a thread pool be used.
        /// </summary>
        public bool UseThreadPool
        {
            get
            {
                return this.useThreadPool;
            }
            set
            {
                this.useThreadPool = value;
            }
        }

        public Chromosome<GENE_TYPE>[] Chromosomes
        {
            get
            {
                return this.chromosomes;
            }
            set
            {
                this.chromosomes = value;
            }
        }

        /// <summary>
        /// How many chromosomes should be created.
        /// </summary>
        private int populationSize;

        /// <summary>
        /// The percent that should mutate.
        /// </summary>
        private double mutationPercent;

        /// <summary>
        /// What percent should be chosen to mate. They will choose partners from the
        /// entire mating population.
        /// </summary>
        private double percentToMate;

        /// <summary>
        /// Percent of the population that the mating population chooses partners.
        /// from.
        /// </summary>
        private double matingPopulation;

        /// <summary>
        /// Should the same gene be prevented from repeating.
        /// </summary>
        private bool preventRepeat;

        /**
         * How much genetic material should be cut when mating.
         */
        private int cutLength;

        /// <summary>
        /// An optional thread pool to use.
        /// </summary>
        private bool useThreadPool;

        /// <summary>
        /// The population.
        /// </summary>
        private Chromosome<GENE_TYPE>[] chromosomes;

        /// <summary>
        /// Get a specific chromosome.
        /// </summary>
        /// <param name="i">The chromosome to return, 0 for the first one.</param>
        /// <returns>A chromosome.</returns>
        public Chromosome<GENE_TYPE> GetChromosome(int i)
        {
            return this.chromosomes[i];
        }

        /// <summary>
        /// Modify the weight matrix and thresholds based on the last call to
        /// calcError.
        /// </summary>
        public void Iteration()
        {

            int countToMate = (int)(this.PopulationSize * this.PercentToMate);
            int offspringCount = countToMate * 2;
            int offspringIndex = this.PopulationSize - offspringCount;
            int matingPopulationSize = (int)(this.populationSize * this.matingPopulation);

            WaitCallback w = new WaitCallback(WorkerProc);
            Random rand = new Random();

            ManualResetEvent[] doneEvents = new ManualResetEvent[countToMate];


            // mate and form the next generation
            for (int i = 0; i < countToMate; i++)
            {
                Chromosome<GENE_TYPE> mother = this.chromosomes[i];
                int fatherInt = (int)(rand.NextDouble() * matingPopulationSize);
                Chromosome<GENE_TYPE> father = this.chromosomes[fatherInt];
                Chromosome<GENE_TYPE> child1 = this.chromosomes[offspringIndex];
                Chromosome<GENE_TYPE> child2 = this.chromosomes[offspringIndex + 1];

                MateWorker<GENE_TYPE> worker = new MateWorker<GENE_TYPE>(
                        mother, father, child1, child2);


                try
                {
                    if (this.useThreadPool)
                    {
                        doneEvents[i] = new ManualResetEvent(false);
                        worker.SetEvent(doneEvents[i]);
                        ThreadPool.QueueUserWorkItem(w, worker);
                    }
                    else
                    {
                        worker.Call();
                    }
                }
                catch (System.Exception)
                {

                }

                // mother.mate(father,chromosomes[offspringIndex],chromosomes[offspringIndex+1]);
                offspringIndex += 2;
            }

            if (this.useThreadPool)
            {
                WaitHandle.WaitAll(doneEvents);
            }

            // sort the next generation
            SortChromosomes();
        }

        /// <summary>
        /// Set the specified chromosome.
        /// </summary>
        /// <param name="i">The chromosome to set.</param>
        /// <param name="value">The value for the specified chromosome.</param>
        public void SetChromosome(int i, Chromosome<GENE_TYPE> value)
        {
            this.chromosomes[i] = value;
        }

  
        /// <summary>
        /// Sort the chromosomes.
        /// </summary>
        public void SortChromosomes()
        {
            Array.Sort(this.chromosomes);
        }

        /// <summary>
        /// Worker method for the threads.
        /// </summary>
        /// <param name="stateInfo">State object to tell the threads what to do.</param>
        private void WorkerProc(Object stateInfo)
        {
            ((MateWorker<GENE_TYPE>)stateInfo).Call();
        }
    }
}
