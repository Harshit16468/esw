#!/usr/bin/env python
import urllib.request
import urllib.error
import json
import datetime
from time import sleep
import sys

READ_API_KEY = 'LX12K2YWH1A42LFZ'
CHANNEL_ID = 2287708

start_time = datetime.datetime.strptime('2014-04-16 07:25:08 UTC', '%Y-%m-%d %H:%M:%S %Z')
results = 5  # fetch this many results (max 8000)

# extract data from thingspeak API, using multiple requests if necessary
# extracts from newest to oldest due to a quirk in the API
# 'results' parameter gives the last n results in the time range rather than the first
# so you will have to re-sort the results again afterward..
def main():
    end_time = datetime.datetime.utcnow()  # or set to desired value like above
    err_count = 0
    while end_time > start_time:
        end_timestr = urllib.parse.quote(datetime.datetime.strftime(end_time, '%Y-%m-%d %H:%M:%S %Z'))
        try:
            conn = urllib.request.urlopen(
                "https://api.thingspeak.com/channels/%d/feeds.csv?results=%d&api_key=%s&end=%s" %
                (CHANNEL_ID, results, READ_API_KEY, end_timestr)
            )

            if conn.getcode() != 200:
                sys.stderr.write("http status code=%d" % (conn.getcode()))
                conn.close()
                err_count += 1
                if err_count > 5:
                    sys.stderr.write("too many errors, giving up")
                    break

                sleep(10)
                continue

            err_count = 0
            response = conn.read()
            data = response.split(b'\n')
            # print in reverse order
            data2 = sorted(filter((lambda x: x != b''), data[1:]), reverse=True)
            if len(data2) <= 0:
                sys.stderr.write("no more data")
                break
            for d in data2:
                print(d.decode('utf-8'))
            # get the last (earliest) timestamp
            dtime = data2[-1].split(b',')[0]
            # subtract 1 second to avoid duplicates
            end_time = datetime.datetime.strptime(dtime.decode('utf-8'), '%Y-%m-%d %H:%M:%S %Z') - datetime.timedelta(
                seconds=1)
            # sys.stderr.write( "end_time= %s" % end_time)

            conn.close()
            sleep(5)

        except urllib.error.URLError as e:
            sys.stderr.write(f"Error: {e.reason}")
            err_count += 1
            if err_count > 5:
                sys.stderr.write("too many errors, giving up")
                break
            sleep(10)
            continue

    sys.stderr.write("finished")


if __name__ == '__main__':
    main()
