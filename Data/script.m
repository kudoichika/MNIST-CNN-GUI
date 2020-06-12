fprintf("\n\nLoading Data....\n");

load('train-img.csv'); % Data to Evaluate On
load('train-lbl.csv'); % Labes for the Data

fprintf("Finished Loading Data\n");
fprintf("Loading Parameters...\n");

%Load the Number of hidden layer params

n_hidden = 1;

thetas = {};

for i = 0:n_hidden
	thetas = [thetas, load(strcat('theta', num2str(i), '.csv'))];
end

fprintf("Finished Loading Parameters.\n");

X = train_img;
y = train_lbl;

m = length(y);

fprintf("Evaluating Network...\n");

function sig = sigmoid(z)
	sig = 1.0 ./ (1.0 + exp(-z));
endfunction

prediction = X;

for i = 1:n_hidden+1
	prediction = sigmoid([ones(m, 1), prediction] * thetas{1,i}');
end

[dummy, p] = max(prediction, [], 2);
[dummy, a] = max(y, [], 2);
eval = sum(a == p);
score = eval/length(y);

fprintf("Finished Evalation. Score = %f\n", score);
