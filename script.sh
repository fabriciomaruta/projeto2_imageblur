echo -e \nTestando main:\n

mkdir out

for file in data/*.jpg; do
	./main "$file" ./out/$(basename $file .jpg)_single_out.jpg
done

echo -e \nTestando process:\n

for file1 in data/*.jpg; do
	./main_p "$file1" ./out/$(basename $file1 .jpg)_process_out.jpg
done

echo -e \nTestando thread:\n

for file2 in data/*.jpg; do
	./main_t "$file2" ./out/$(basename $file2 .jpg)_thread_out.jpg
done
