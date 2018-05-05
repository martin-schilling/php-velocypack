FROM extension-builder

COPY modules/velocypack.so /tmp

RUN echo "extension=velocypack.so" > /usr/local/etc/php/conf.d/velocypack.ini \
    && mv /tmp/velocypack.so $(php-config --extension-dir)
