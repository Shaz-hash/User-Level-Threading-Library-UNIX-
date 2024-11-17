from typing import List
from sys import argv
import os
import subprocess
from glob import glob


class Grader:
    def __init__(self):
        self.score = 0
        self.thread_score = 0
        self.mutex_score = 0
        self.semaphore_score = 0
        self.doctor_score = 0
        self.laundry_score = 0
        self.thread = False
        self.mutex = False
        self.semaphore = False
        self.doctor = False
        self.laundry = False
        self.cwd = os.getcwd()
        self.create_output_dirs()
        self.clean_build()

    def set_test(self, test: List[bool]):
        self.thread = test[0]
        self.mutex = test[1]
        self.semaphore = test[2]
        self.doctor = test[3]
        self.laundry = test[4]

    def clean_build(self):
        try:
            subprocess.run("make clean", cwd=f"{self.cwd}", shell=True, check=True)
        except Exception as e:
            print(f"Error running make clean {e}")
            return

    def create_output_dirs(self):
        dirs = ["thread", "mutex", "semaphore", "doctor", "laundry"]

        for dir in dirs:
            try:
                os.mkdir(f"{self.cwd}/test/{dir}/student_output/")
            except:
                pass


    def display_score(self):
        print(f"============ Scoring Breakdown ============")

        if self.thread:
            print(f"Thread: {self.thread_score}")
        if self.mutex:
            print(f"Mutex: {self.mutex_score}")
        if self.semaphore:
            print(f"Semaphore: {self.semaphore_score}")
        if self.doctor:
            print(f"Doctor: {self.doctor_score}")
        if self.laundry:
            print(f"Laundry: {self.laundry_score}")

    def write_score_to_file(self):
        with open("score.txt", "w") as f:
            f.write("Score: ")
            f.write(str(self.score))
            f.write("\n\n============ Scoring Breakdown ============\n")
            if self.thread:
                f.write(f"Thread: {self.thread_score}\n")
            if self.mutex:
                f.write(f"Mutex: {self.mutex_score}\n")
            if self.semaphore:
                f.write(f"Semaphore: {self.semaphore_score}\n")
            if self.doctor:
                f.write(f"Doctor: {self.doctor_score}\n")
            if self.laundry:
                f.write(f"Laundry: {self.laundry_score}\n")


    def test_thread(self):
        total_score = 50

        # run make in /test/thread
        try:
            subprocess.run("make thread", cwd=f"{self.cwd}", shell=True, check=True)
        except Exception as e:
            print(f"Error running make {e}")
            return

        path_to_executables = f"{self.cwd}/test/thread/build/"
        path_to_student_output = f"{self.cwd}/test/thread/student_output/"
        executables = glob(path_to_executables + "*")

        # test_scores = [10, 10, 10, 20]
        test_scores = [total_score/len(executables) for _ in range(len(executables))]

        for executable in executables:
            try:
                subprocess.run(
                    args=f"{executable} | tee {path_to_student_output + executable.split('/')[-1].split('.')[0] + '.output'}",
                    timeout=5,
                    shell=True,
                )
            except Exception as e:
                print(f"Error running {executable}: {e}")


        # compare each output file with the correct output file, stored in /test/thread/output
        # if correct, add score to total score
        correct_files = glob("test/thread/outputs/*")

        try:
            for file in correct_files:
                try: # add exceptiopn handling for each file
                    give_marks = True
                    file_num = file.split(".")[0].split("/")[-1]
                    correct = open(file, "r")
                    student = open(path_to_student_output + file_num + ".output", "r")

                    correct_lines = correct.readlines()
                    student_lines = student.readlines()

                    for index, line in enumerate(correct_lines):
                        if line != student_lines[index]:
                            print(f"Line {index} in file {file_num} does not match")
                            give_marks = False

                    correct.close()
                    student.close()

                    if give_marks:
                        self.score += test_scores[int(file_num[-1]) - 1]
                        self.thread_score += test_scores[int(file_num[-1]) - 1]
                except:
                    print(f"Error running test_thread. File {file_num} does not match")
        except:
            print("Error running test_thread. Number of output files does not match")

        self.score = int(self.score)
        self.thread_score = int(self.thread_score)

    def test_mutex(self):
        total_score = 7.5
        # test_scores = [2.5, 2.5, 2.5]

        path_to_executables = f"{self.cwd}/test/mutex/build/"
        path_to_student_output = f"{self.cwd}/test/mutex/student_output/"

        try:
            subprocess.run("make mutex", cwd=f"{self.cwd}", shell=True, check=True)
        except Exception as e:
            print(f"Error running make {e}")
            return

        executables = glob(path_to_executables + "*")
        test_scores = [total_score/len(executables) for _ in range(len(executables))]

        for executable in executables:
            try:
                subprocess.run(
                    args=f"{executable} | tee {path_to_student_output + executable.split('/')[-1].split('.')[0] + '.output'}",
                    timeout=5,
                    shell=True,
                )
            except Exception as e:
                print(f"Error running {executable}: {e}")

        # compare each output file with the correct output file, stored in /test/mutex/output
        # if correct, add score to total score
        correct_files = glob("test/mutex/outputs/*")

        try:
            for file in correct_files:
                try:
                    give_marks = True
                    file_num = file.split(".")[0].split("/")[-1]
                    correct = open(file, "r")
                    student = open(path_to_student_output + file_num + ".output", "r")

                    correct_lines = correct.readlines()
                    student_lines = student.readlines()

                    for index, line in enumerate(correct_lines):
                        if line != student_lines[index]:
                            print(f"Line {index} in file {file_num} does not match")
                            give_marks = False

                    correct.close()
                    student.close()

                    if give_marks:
                        self.score += test_scores[int(file_num[-1]) - 1]
                        self.mutex_score += test_scores[int(file_num[-1]) - 1]
                except:
                    print(f"Error running test_mutex. File {file_num} does not match")
        except:
            print("Error running test_mutex. Number of output files does not match")

    def test_semaphore(self):
        total_score = 12.5

        # list all test files
        path_to_executables = f"{self.cwd}/test/semaphore/build/"
        path_to_student_output = f"{self.cwd}/test/semaphore/student_output/"

        try:
            subprocess.run("make semaphore", cwd=f"{self.cwd}", shell=True, check=True)
        except Exception as e:
            print(f"Error running make {e}")
            return

        executables = glob(path_to_executables + "*")

        test_scores = [total_score/len(executables) for _ in range(len(executables))]

        for executable in executables:
            try:
                subprocess.run(
                    args=f"{executable} | tee {path_to_student_output + executable.split('/')[-1].split('.')[0] + '.output'}",
                    timeout=5,
                    shell=True,
                )
            except Exception as e:
                print(f"Error running {executable}: {e}")

        # compare each output file with the correct output file, stored in /test/semaphore/output
        # if correct, add score to total score
        correct_files = glob("test/semaphore/outputs/*")
        try:
            for file in correct_files:
                try:
                    give_marks = True
                    file_num = file.split(".")[0].split("/")[-1]
                    correct = open(file, "r")
                    student = open(path_to_student_output + file_num + ".output", "r")

                    correct_lines = correct.readlines()
                    student_lines = student.readlines()

                    for index, line in enumerate(correct_lines):
                        if line != student_lines[index]:
                            print(f"Line {index} in file {file_num} does not match")
                            give_marks = False

                    correct.close()
                    student.close()

                    if give_marks:
                        self.score += test_scores[int(file_num[-1]) - 1]
                        self.semaphore_score += test_scores[int(file_num[-1]) - 1]
                except:
                    print(f"Error running test_semaphore. File {file_num} does not match")
        except:
            print("Error running test_semaphore. Number of output files does not match")

    def test_laundry(self):
        total_score = 15
        # scores = [3] * 5

        try:
            subprocess.run("make laundry", cwd=f"{self.cwd}", shell=True, check=True)
        except Exception as e:
            print(f"Error running make {e}")
            return

        path_to_executable = f"{self.cwd}/test/laundry/build/laundry.out"
        path_to_input = f"{self.cwd}/test/laundry/inputs/"
        path_to_correct_output = f"{self.cwd}/test/laundry/outputs/"
        path_to_student_output = f"{self.cwd}/test/laundry/student_output/"

        input_files = glob(path_to_input + "*")
        
        for file in input_files:
            file_num = file.split("/")[-1].split(".")[0]
            try:
                subprocess.run(
                    args=f"{path_to_executable} < {file} | tee {path_to_student_output + file_num + '.out'}",
                    # stdin=open(file, "r"),
                    # stdout=open(path_to_student_output + file_num + ".out", "w"),
                    timeout=10,
                    shell=True,
                )
            except Exception as e:
                print(f"Error running {file}: {e}")

        correct_output_files = glob(path_to_correct_output + "*")
        scores = [total_score/len(correct_output_files) for _ in range(len(correct_output_files))]

        try:
            for index, file in enumerate(correct_output_files):
                try:
                    give_marks = True
                    file_num = file.split("/")[-1].split(".")[0]
                    correct = open(file, "r")
                    student = open(path_to_student_output + file_num + ".out", "r")

                    correct_lines = correct.readlines()
                    student_lines = student.readlines()

                    for index, line in enumerate(correct_lines):
                        if line != student_lines[index]:
                            print(f"Line {index} in file {file_num} does not match")
                            continue

                    correct.close()
                    student.close()
                    if give_marks:
                        self.score += scores[int(file_num) - 1]
                        self.laundry_score += scores[int(file_num) - 1]
                except:
                    print(f"Error running test_laundry. File {file_num} does not match")
                    continue

        except:
            print("Error running test_laundry. Number of output files does not match")

    def test_doctor(self):
        total_score = 15

        try:
            subprocess.run("make doctor", cwd=f"{self.cwd}", shell=True, check=True)
        except Exception as e:
            print(f"Error running make {e}")
            return

        path_to_executable = f"{self.cwd}/test/doctor/build/doctor.out"
        path_to_input = f"{self.cwd}/test/doctor/inputs/"
        path_to_correct_output = f"{self.cwd}/test/doctor/outputs/"
        path_to_student_output = f"{self.cwd}/test/doctor/student_output/"

        input_files = glob(path_to_input + "*")
        correct_output_files = glob(path_to_correct_output + "*")

        scores = [total_score/len(correct_output_files) for _ in range(len(correct_output_files))]

        for file in input_files:
            file_num = file.split("/")[-1].split(".")[0]
            try:
                subprocess.run(
                    args=f"{path_to_executable} | tee {path_to_student_output + file_num + '.out'}",
                    stdin=open(file, "r"),
                    # stdout=open(path_to_student_output + file_num + ".out", "w"),
                    timeout=10,
                    shell=True,
                )
            except Exception as e:
                print(f"Error running {file}: {e}")

        try:
            for file in correct_output_files:
                try:
                    give_marks = True
                    file_num = file.split("/")[-1].split(".")[0]
                    correct = open(file, "r")
                    student = open(path_to_student_output + file_num + ".out", "r")

                    correct_lines = correct.readlines()
                    student_lines = student.readlines()

                    for index, line in enumerate(correct_lines):
                        if line != student_lines[index]:
                            print(f"Line {index} in file {file_num} does not match")
                            give_marks = False
                            

                    correct.close()
                    student.close()

                    if give_marks:
                        self.score += scores[int(file_num) - 1]
                        self.doctor_score += scores[int(file_num) - 1]
                except Exception as e:
                    print(f"Error running test_doctor. File {e} does not match")
        except:
            print("Error running test_doctor. Number of output files does not match")

    def run(self):
        if self.thread:
            print("Testing Threads")
            self.test_thread()
            # print(f"Score: {self.score}\n")
        if self.mutex:
            print("Testing Mutex")
            self.test_mutex()
            # print(f"Score: {self.score}\n")

        if self.semaphore:
            print("Testing Semaphore")
            self.test_semaphore()
            # print(f"Score: {self.score}\n")

        if self.doctor:
            print("Testing Doctor")
            self.test_doctor()
            # print(f"Score: {self.score}\n")

        if self.laundry:
            print("Testing Laundry")
            self.test_laundry()
            # print(f"Score: {self.score}\n")


        self.display_score()
        print(f"Testing Completed. Final Score: {self.score}")
        self.write_score_to_file()


def main():
    # Initialize a list of bools, all set to false
    flags = [False] * 5

    # Check for command line arguments
    for arg in argv[1:]:
        if arg == "-t":
            flags[0] = True
        elif arg == "-m":
            flags[1] = True
        elif arg == "-s":
            flags[2] = True
        elif arg == "-d":
            flags[3] = True
        elif arg == "-l":
            flags[4] = True

    if len(argv) == 1:
        flags = [True] * 5

    # Initialize a grader object
    grader = Grader()
    grader.set_test(flags)
    print("Running tests...")
    grader.run()


if __name__ == "__main__":
    main()
