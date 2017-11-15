#include "stdafx.h"

#include "NetworkTrainer.h"

NetworkTrainer::NetworkTrainer() {
}

NetworkTrainer::~NetworkTrainer() {
}

int NetworkTrainer::trainNetwork() {
	try
	{

		std::cout << "OpenNN. MIXED DATASET" << std::endl;

		srand((unsigned int)time(NULL));

		DataSet data_set;
		data_set.set_data_file_name("data.dat");
		data_set.set_separator("Comma");
		cout << "about to load data" << endl;
		data_set.load_data();
		cout << "loaded data" << endl;

		/*VARIABLES */

		Variables* variables_pointer = data_set.get_variables_pointer();

		variables_pointer->set_name(0, "Mean_Acc400_Centroid_Power_powerFFT_WinHamming_ real");
		variables_pointer->set_use(0, Variables::Input);

		variables_pointer->set_name(1, "Mean_Acc400_Rolloff_Power_powerFFT_WinHamming_ real");
		variables_pointer->set_use(1, Variables::Input);

		variables_pointer->set_name(2, "Mean_Acc400_MFCC0_Power_powerFFT_WinHamming_ real");
		variables_pointer->set_use(2, Variables::Input);

		variables_pointer->set_name(3, "Mean_Acc400_MFCC1_Power_powerFFT_WinHamming_ real");
		variables_pointer->set_use(3, Variables::Input);

		variables_pointer->set_name(4, "Mean_Acc400_MFCC2_Power_powerFFT_WinHamming_ real");
		variables_pointer->set_use(4, Variables::Input);

		variables_pointer->set_name(5, "Mean_Acc400_MFCC3_Power_powerFFT_WinHamming_ real");
		variables_pointer->set_use(5, Variables::Input);

		variables_pointer->set_name(6, "Mean_Acc400_MFCC4_Power_powerFFT_WinHamming_ real");
		variables_pointer->set_use(6, Variables::Input);

		variables_pointer->set_name(7, "Mean_Acc400_MFCC5_Power_powerFFT_WinHamming_ real");
		variables_pointer->set_use(7, Variables::Input);

		variables_pointer->set_name(8, "Mean_Acc400_MFCC6_Power_powerFFT_WinHamming_ real");
		variables_pointer->set_use(8, Variables::Input);

		variables_pointer->set_name(9, "Mean_Acc400_MFCC7_Power_powerFFT_WinHamming_ real");
		variables_pointer->set_use(9, Variables::Input);

		variables_pointer->set_name(10, "Mean_Acc400_MFCC8_Power_powerFFT_WinHamming_ real");
		variables_pointer->set_use(10, Variables::Input);

		variables_pointer->set_name(11, "Mean_Acc400_MFCC9_Power_powerFFT_WinHamming_ real");
		variables_pointer->set_use(11, Variables::Input);

		variables_pointer->set_name(12, "Mean_Acc400_MFCC10_Power_powerFFT_WinHamming_ real");
		variables_pointer->set_use(12, Variables::Input);

		variables_pointer->set_name(13, "Mean_Acc400_MFCC11_Power_powerFFT_WinHamming_ real"); //WTF
		variables_pointer->set_use(13, Variables::Input);

		variables_pointer->set_name(14, "Mean_Acc400_MFCC12_Power_powerFFT_WinHamming_ real");
		variables_pointer->set_use(14, Variables::Input);

		variables_pointer->set_name(15, "Mean_Acc400_Kurtosis real"); // WTF
		variables_pointer->set_use(15, Variables::Input);

		variables_pointer->set_name(16, "Mean_Acc400_Skewness real"); //WTF
		variables_pointer->set_use(16, Variables::Input);

		variables_pointer->set_name(17, "Peak Ratio Chroma D");
		variables_pointer->set_use(17, Variables::Input);

		variables_pointer->set_name(18, "Peak Ratio Min Chroma A 3");
		variables_pointer->set_use(18, Variables::Input); //WTF

		variables_pointer->set_name(19, "Zero Crossings"); //WTF
		variables_pointer->set_use(19, Variables::Input);

		variables_pointer->set_name(20, "MFCC 10"); // WTF
		variables_pointer->set_use(20, Variables::Input);

		variables_pointer->set_name(21, "Center");
		variables_pointer->set_use(21, Variables::Target);

		variables_pointer->set_name(22, "Halfedge");
		variables_pointer->set_use(22, Variables::Target);

		variables_pointer->set_name(23, "Rimshot");
		variables_pointer->set_use(23, Variables::Target);

		const Matrix<std::string> inputs_information = variables_pointer->arrange_inputs_information();
		const Matrix<std::string> targets_information = variables_pointer->arrange_targets_information();

		/*SCALING THE INPUT DATA FOR BETTER PERFORMANCE*/
		Instances* instances_pointer = data_set.get_instances_pointer();

		instances_pointer->split_random_indices();

		const Vector< Statistics<double> > inputs_statistics = data_set.scale_inputs_minimum_maximum();

		/*SCALING THE INPUT DATA FOR APPROXIMATION PROBLEMS*/
		const Vector< Statistics<double> > targets_statistics = data_set.scale_targets_minimum_maximum();

		/*OBTAIN AND SET INFO ABOUT THE VARIABLES OF THE DATA*/

		const size_t inputs_number = variables_pointer->count_inputs_number();//should give 20
		cout << "\n inputs number : " << inputs_number << endl;
		const size_t outputs_number = variables_pointer->count_targets_number(); //should give 3
		cout << "\n outputs number: " << outputs_number << endl;

		/*----------------------------------*/

		NeuralNetwork neural_network(inputs_number, 30, outputs_number);

		neural_network.construct_scaling_layer();

		ScalingLayer* scaling_layer_pointer = neural_network.get_scaling_layer_pointer();

		scaling_layer_pointer->set_statistics(inputs_statistics);

		scaling_layer_pointer->set_scaling_method(ScalingLayer::NoScaling);

		/*PROBABILISTIC LAYER - FOR PATTERN RECOGNITION & CLASSIFICATION NN'S*/

		neural_network.construct_probabilistic_layer();

		ProbabilisticLayer* probability_layer_pointer = neural_network.get_probabilistic_layer_pointer();

		probability_layer_pointer->set_probabilistic_method(ProbabilisticLayer::Probability);

		/*ERROR*/

		LossIndex loss_index(&neural_network, &data_set);//set both pointers at same time!
		loss_index.set_error_type("NORMALIZED_SQUARED_ERROR"); //not in the example.
		loss_index.set_regularization_type(LossIndex::NEURAL_PARAMETERS_NORM);

		/*TRAINING STRATEGY*/
		TrainingStrategy training_strategy(&loss_index);
		training_strategy.set_main_type(TrainingStrategy::QUASI_NEWTON_METHOD);
		QuasiNewtonMethod* quasi_Newton_method_pointer = training_strategy.get_quasi_Newton_method_pointer();
		quasi_Newton_method_pointer->set_maximum_iterations_number(15); //changed this.
		quasi_Newton_method_pointer->set_display_period(2);
		training_strategy.perform_training();

		TestingAnalysis testing_analysis(&neural_network, &data_set);
		Matrix<size_t> confusion = testing_analysis.calculate_confusion();

		scaling_layer_pointer->set_scaling_method(ScalingLayer::MinimumMaximum);

		neural_network.save_expression("proceeds/expresion.txt");
		data_set.save("proceeds/data_set.xml");

		neural_network.save("proceeds/neural_network.xml");
		neural_network.save_expression("proceeds/expression.txt");
		confusion.save("proceeds/confusion.dat");

		training_strategy.save("proceeds/training_strategy.xml");

		return(0);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return(1);
	}
}