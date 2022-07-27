#pragma once
#include <vector>
#include "Layer.h"

class Optimizer_SGD
{
public:
	Optimizer_SGD(float startingLearningRate, float learningRateDecay);
	void UpdateParams(Layer& layer);
	float GetLearningRate() const { return m_LearningRate; }
private:
	float m_LearningRate;
	float m_LearningRateDecay;
	int m_IterationCount;
};

class NeuralNetwork
{
public:
	NeuralNetwork();
	void AddLayer(unsigned int numInputs, unsigned int size);
	inline Layer& GetLayer(unsigned int index) { return m_Layers[index]; }
	inline Eigen::MatrixXf GetOutput() const { return m_CurrentOutput; }
	inline int GetNumLayers() const { return m_Layers.size(); }

	void ForwardProp(Eigen::MatrixXf* input);
	void BackwardProp(const Eigen::MatrixXf& yTrue);
	void Fit(Eigen::MatrixXf input, const Eigen::MatrixXf& y, Optimizer_SGD& optimizer);

	void Optimize(Optimizer_SGD& optimizer);
	float CalculateLoss(const Eigen::MatrixXf& yTrue);
	float CalculateAccuracy(Eigen::VectorXf yTrue);

	Eigen::VectorXf GetQs(const Eigen::VectorXf& input);
	Eigen::MatrixXf GetQs(const Eigen::MatrixXf& input);
	int Predict(const Eigen::VectorXf& input);
	void Print();
	void SaveWeights(const std::string& fileName = "Weights.txt");
	void LoadWeights(const std::string& fileName = "Weights.txt");

private:
	std::vector<Layer> m_Layers;
	Eigen::MatrixXf m_CurrentOutput;
};

