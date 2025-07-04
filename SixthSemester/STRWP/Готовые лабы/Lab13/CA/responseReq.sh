openssl x509 -req -in RS-LAB22-KAV.csr -CA CA-LAB22-ZSS.crt -CAkey CA-LAB22-ZSS.key -CAcreateserial -out RS-LAB22-KAV.crt -days 7 -sha256 -extensions v3_req -extfile cfg.cfg
