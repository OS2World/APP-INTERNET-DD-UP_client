DD-UP by Thomas Gandy and Tom Groves (support@ddup.org)
---------------------------------------------------------

DD-UP is a program that is used to update a host
provided by the Free DynDNS service of dyndns.org.

The software is distributed under the GNU General Public Liscense because
it uses code from glibc for cross platform compatiblity.

INSTALL: instructions for installing DD-UP.

ddupcron.sh is a cron-able script that will check the current IP address
on the network device (using /sbin/ifconfig) and stores it for future
reference (edit ddupcron.sh for the correct interface).

Usage: "ddup --help" will get you usage

A little note on MX: MX stands for Mail Exchanger.  To use this feature
you must find a host such as your Internet Service Provider that will
setup their e-mail server to recieve mail for your domain.  If you haven't
made arrangements with someone this field will not work PLEASE don't use
it if you don't have a MX setup on a different host.
