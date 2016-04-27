(function() {
    'use strict';

    angular.module('efb').directive('navbar', function() {
        return {
            templateUrl: 'client/navbar/navbar.template.html',
            restrict: 'E',
            controller: 'NavbarController',
            controllerAs: 'NavbarCtrl'
        };
    });
})();