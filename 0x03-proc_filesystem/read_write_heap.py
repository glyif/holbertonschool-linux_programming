#!/usr/bin/python3
"""
SearchReplaceHeap:
search and replaces in a heap of a program
"""

from sys import argv, exit


class SearchReplaceHeap:
    def __init__(self, pid, search_string, replace_string):
        """
        init
        :param pid: pid of running process
        :param search_string: string to search for
        :param replace_string: string to replace searched string
        """
        self.pid = pid
        self.search_string = bytes(search_string, "ASCII")
        self.replace_string = bytes(replace_string, "ASCII")
        self.null_bytes = bytes('\0' * (len(self.search_string) -
                                        len(self.replace_string)), "ASCII")
        self.start_address = ""
        self.end_address = ""

    def search_replace(self):
        """
        search_replace - searches and replaces a string in heap
        """
        maps_file = self.validate_file("maps", "r")
        self.read_maps_file(maps_file)

        mem_file = self.validate_file("mem", "rb+")
        self.read_replace_mem(mem_file)

    def read_maps_file(self, maps_file):
        """
        read_maps_file - read maps file
        :param maps_file: opened maps file
        """
        for line in maps_file:
            if "[heap]" in line:
                split_line = line.split(' ')
                addresses = split_line[0].split('-')
                self.start_address = int(addresses[0], 16)
                self.end_address = int(addresses[1], 16)
                break
        maps_file.close()

    def read_replace_mem(self, mem_file):
        """
        reads mem file and replaces string
        :param mem_file:
        """
        mem_file.seek(self.start_address)
        heap = mem_file.read(self.end_address - self.start_address)

        found_string = heap.index(self.search_string)

        mem_file.seek(self.start_address + found_string)
        mem_file.write(self.replace_string + self.null_bytes)
        mem_file.close()

    @staticmethod
    def validate_args():
        """
        validate_args - validates arguments
        """
        invalid_argument = ""
        if len(argv) != 4:
            invalid_argument = "invalid number of args"
        elif int(argv[1]) < 1:
            invalid_argument = "pid"
        elif len(argv[2]) <= 0:
            invalid_argument = "search_string"
        elif len(argv[3]) <= 0:
            invalid_argument = "replace_string"

        if len(invalid_argument) > 0:
            SearchReplaceHeap.print_error(invalid_argument)

    def validate_file(self, file, permission):
        """
        validate_file - validates if a file can be opened
        :param file: file to open
        :param permission: opermisino of files
        :return: opened file or exception error
        """
        try:
            file = open("/proc/{:s}/{:s}".format(self.pid, file), permission)
            return file
        except IOError:
            SearchReplaceHeap.print_error("unable to open {:s} file,"
                                          "maybe invalid pid: {:s} or file"
                                          .format(file, self.id))

    @staticmethod
    def print_error(error):
        """
        print_error - prints error and exits
        :param error: error to print
        """
        print("Error: {:s}".format(error))
        exit(1)


SearchReplaceHeap.validate_args()
search_replace = SearchReplaceHeap(argv[1], argv[2], argv[3])
search_replace.search_replace()
exit(0)
