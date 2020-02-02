bazel run apps/samples/voice_command_detection:training -- --train_dataset_path ~/ml/audio/training/ --validation_dataset_path ~/ml/audio/validation/ -n --noise_profile_path ~/ml/audio/noise/ --keywords_list jetson,left,right --keyword_duration 0.7 --model_output_path ~/ml/audio/model --training_epochs 500

bazel run apps/voice_test:training_denoise -- --train_dataset_path ~/ml/audio/training-denoise/ --validation_dataset_path ~/ml/audio/validation-denoise/ --keyword_duration 0.7