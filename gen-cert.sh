#!/bin/bash
openssl dhparam -out dh.pem 2048
openssl req -newkey rsa:2048 -nodes -keyout key.pem -x509 -days 100000 -out cert.pem -subj "//C=US\ST=CA\L=LosAngeles\O=SLXS\CN=www.tax.gov.ir"
