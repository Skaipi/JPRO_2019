/* Save simulation state to file */
void SaveSimulation(FluidBlock* block);

/* Get state of simulation */
void GetCurrentState(FILE* file);

/* Get values */
float GetAverageVelocity(FluidBlock* block);
float GetAverageDensity(FluidBlock* block);
float GetFilledSpace(FluidBlock* block);