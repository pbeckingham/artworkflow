import atexit
import datetime
import json
import os
import shlex
import shutil
import tempfile
import unittest

from .exceptions import CommandError
from .utils import run_cmd_wait, run_cmd_wait_nofail, which, shared_binary_location, DEFAULT_EXTENSION_PATH


class artworkflow(object):
    """
    Manage an artworkflow instance

    A temporary folder is used as data store.

    An artworkflow client should not be used after being destroyed.
    """
    DEFAULT_ARTWORKFLOW = shared_binary_location()

    def __init__(self, artworkflow=DEFAULT_ARTWORKFLOW):
        """
        Initialize an artworkflow (client).
        The program runs in a temporary folder.

        :arg artworkflow: artworkflow binary to use as client (defaults: artworkflow in PATH)
        """
        self.artworkflow = artworkflow

        # Used to specify what command to launch (and to inject faketime)
        self._command = [self.artworkflow]

        # Configuration of the isolated environment
        self._original_pwd = os.getcwd()
        self.datadir = tempfile.mkdtemp(prefix="artworkflow")
        self.artworkflowrc = os.path.join(self.datadir, "artworkflow.cfg")
        self.extdir = os.path.join(self.datadir, "extensions")

        # Ensure any instance is properly destroyed at session end
        atexit.register(lambda: self.destroy())

        self.reset_env()

    def add_default_extension(self, filename):
        """Add default extension to current instance"""
        if not os.path.isdir(self.extdir):
            os.mkdir(self.extdir)

        extfile = os.path.join(self.extdir, filename)
        if os.path.isfile(extfile):
            raise "{} already exists".format(extfile)

        shutil.copy(os.path.join(DEFAULT_EXTENSION_PATH, filename), extfile)

    def __repr__(self):
        txt = super(artworkflow, self).__repr__()
        return "{0} running from {1}>".format(txt[:-1], self.datadir)

    def __call__(self, *args, **kwargs):
        """aka t = artworkflow() ; t() which is now an alias to t.runSuccess()"""
        return self.runSuccess(*args, **kwargs)

    def reset_env(self):
        """Set a new environment derived from the one used to launch the test"""
        # Copy all env variables to avoid clashing subprocess environments
        self.env = os.environ.copy()

        # As well as TIMEWARRIORDB
        self.env["TIMEWARRIORDB"] = self.datadir

        # As well as MANPATH, so that the help tests can find the
        # uninstalled man pages
        parts = self.artworkflow.split(os.path.sep)[0:-2]
        parts.append("doc")
        self.env["MANPATH"] = os.path.sep.join(parts)

    def config(self, var, value):
        """Run setup `var` as `value` in artworkflow config"""
        cmd = (self.artworkflow, ":yes", "config", var, value)
        return run_cmd_wait(cmd, env=self.env)

    @staticmethod
    def _create_exclusion_interval(interval):
        if not isinstance(interval, tuple):
            raise TypeError("Please specify interval(s) as a tuple or a list of tuples")

        if interval[0] is not None and not isinstance(interval[0], datetime.time):
            raise TypeError("Start date must be a datetime.time but is {}".format(type(interval[0])))

        if interval[1] is not None and not isinstance(interval[1], datetime.time):
            raise TypeError("End date must be a datetime.time but is {}".format(type(interval[1])))

        if interval[0] is None:
            return "<{:%H:%M:%S}".format(interval[1])

        if interval[1] is None:
            return ">{:%H:%M:%S}".format(interval[0])

        if interval[0] > interval[1]:
            return "<{:%H:%M:%S} >{:%H:%M:%S}".format(interval[1], interval[0])

        return "{:%H:%M:%S}-{:%H:%M:%S}".format(interval[0], interval[1])

    def configure_exclusions(self, intervals):
        if isinstance(intervals, list):
            exclusion = " ".join([self._create_exclusion_interval(interval) for interval in intervals])

        else:
            exclusion = self._create_exclusion_interval(intervals)

        self.config("exclusions.monday", exclusion)
        self.config("exclusions.tuesday", exclusion)
        self.config("exclusions.wednesday", exclusion)
        self.config("exclusions.thursday", exclusion)
        self.config("exclusions.friday", exclusion)
        self.config("exclusions.saturday", exclusion)
        self.config("exclusions.sunday", exclusion)

    def del_config(self, var):
        """Remove `var` from artworkflow config"""
        cmd = (self.artworkflow, ":yes", "config", var)
        return run_cmd_wait(cmd, env=self.env)

    @property
    def artworkflowrc_content(self):
        """Returns the contents of the artworkflowrc file."""

        with open(self.artworkflowrc, "r") as f:
            return f.readlines()

    def export(self, export_filter=None):
        """Run "artworkflow export", return JSON array of exported intervals."""
        if export_filter is None:
            export_filter = ""

        code, out, err = self.runSuccess("{0} export".format(export_filter))

        return json.loads(out)

    @staticmethod
    def _split_string_args_if_string(args):
        """
        Helper function to parse and split into arguments a single string argument.
        The string is literally the same as if written in the shell.
        """
        # Enable nicer-looking calls by allowing plain strings
        if isinstance(args, str):
            args = shlex.split(args)

        return args

    def runSuccess(self, args="", input=None, merge_streams=False, timeout=5):
        """
        Invoke artworkflow with given arguments and fail if exit code != 0

        Use runError if you want exit_code to be tested automatically and
        *not* fail if program finishes abnormally.

        If you wish to pass instructions to artworkflow such as confirmations or other
        input via stdin, you can do so by providing an input string.
        Such as input="y\ny\n".

        If merge_streams=True stdout and stderr will be merged into stdout.

        timeout = number of seconds the test will wait for every artworkflow call.
        Defaults to 1 second if not specified. Unit is seconds.

        Returns (exit_code, stdout, stderr) if merge_streams=False
                (exit_code, output) if merge_streams=True
        """
        # Create a copy of the command
        command = self._command[:]

        args = self._split_string_args_if_string(args)
        command.extend(args)

        output = run_cmd_wait_nofail(command, input,
                                     merge_streams=merge_streams,
                                     env=self.env,
                                     timeout=timeout)

        if output[0] != 0:
            raise CommandError(command, *output)

        return output

    def runError(self, args=(), input=None, merge_streams=False, timeout=5):
        """
        Invoke artworkflow with given arguments and fail if exit code == 0

        Use runSuccess if you want exit_code to be tested automatically and
        *fail* if program finishes abnormally.

        If you wish to pass instructions to artworkflow such as confirmations or other
        input via stdin, you can do so by providing an input string.
        Such as input="y\ny\n".

        If merge_streams=True stdout and stderr will be merged into stdout.

        timeout = number of seconds the test will wait for every artworkflow call.
        Defaults to 1 second if not specified. Unit is seconds.

        Returns (exit_code, stdout, stderr) if merge_streams=False
                (exit_code, output) if merge_streams=True
        """
        # Create a copy of the command
        command = self._command[:]

        args = self._split_string_args_if_string(args)
        command.extend(args)

        output = run_cmd_wait_nofail(command, input,
                                     merge_streams=merge_streams,
                                     env=self.env,
                                     timeout=timeout)

        # output[0] is the exit code
        if output[0] == 0 or output[0] is None:
            raise CommandError(command, *output)

        return output

    def destroy(self):
        """Cleanup the data folder and release server port for other instances"""
        try:
            shutil.rmtree(self.datadir)
        except OSError as e:
            if e.errno == 2:
                # Directory no longer exists
                pass
            else:
                raise

        # Prevent future reuse of this instance
        self.runSuccess = self.__destroyed
        self.runError = self.__destroyed

        # self.destroy will get called when the python session closes.
        # If self.destroy was already called, turn the action into a noop
        self.destroy = lambda: None

    def __destroyed(self, *args, **kwargs):
        raise AttributeError("Program instance has been destroyed. "
                             "Create a new instance if you need a new client.")

    def faketime(self, faketime=None):
        """
        Set a faketime using libfaketime that will affect the following command calls.

        If faketime is None, faketime settings will be disabled.
        """
        cmd = which("faketime")
        if cmd is None:
            raise unittest.SkipTest("libfaketime/faketime is not installed")

        if self._command[0] == cmd:
            self._command = self._command[3:]

        if faketime is not None:
            # Use advanced time format
            self._command = [cmd, "-f", faketime] + self._command
