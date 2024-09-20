Java SpringCloud Bibbon<br />RestTemplate.java<br />
```java
package com.fcant.userservice.controller;

import com.fcant.userservice.bean.Country;
import com.fcant.userservice.bean.User;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.client.RestClientException;
import org.springframework.web.client.RestTemplate;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * RestTemplateController
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 10:06 2019/12/9
 */
@RestController
@RequestMapping("/user")
public class RestTemplateController {

    private static final Logger LOGGER = LoggerFactory.getLogger(RestTemplateController.class);
    private static final List<User> USER_LIST;

    static {
        List<User> userList = new ArrayList<>();
        userList.add(new User(1L, "User 01", "user01@fcant.com", "男", 1L, ""));
        userList.add(new User(2L, "User 02", "user02@fcant.com", "女", 2L, ""));
        USER_LIST = Collections.unmodifiableList(userList);
    }

    private RestTemplate restTemplate;

    @Autowired
    public RestTemplateController(RestTemplate restTemplate) {
        this.restTemplate = restTemplate;
    }

    @GetMapping("/rest-template")
    public List<User> queryUser(){
        List<User> userList = new ArrayList<>(USER_LIST);
        userList.forEach(user -> {
            Country country = null;
            try {
                country = restTemplate.getForObject("http://area-service/country/{countryId}",
                        Country.class,
                        user.getCountryId());
                user.setCountryName(country.getCountryName());
            } catch (RestClientException e) {
                e.printStackTrace();
            }
        });
        return userList;
    }
}

```

