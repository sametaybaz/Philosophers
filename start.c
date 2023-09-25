/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaybaz <aaybaz@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 13:40:05 by aaybaz            #+#    #+#             */
/*   Updated: 2023/06/16 15:02:06 by aaybaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdio.h>

void	exit_launcher(t_game_rules *rules, t_philo *philos)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	while (++i < rules->num_philos)
		pthread_join(philos[i].thread_id, NULL);
	while (++j < rules->num_philos)
		pthread_mutex_destroy(&(rules->forks[j]));
	pthread_mutex_destroy(&(rules->write_status));
	pthread_mutex_destroy(&(rules->check_eat));
	free(rules->philosophers);
	free(rules->forks);
}

void	check_death(t_game_rules *r, t_philo *p)
{
	int	i;

	while (!(r->all_philos_ate))
	{
		i = -1;
		while (++i < r->num_philos && !(r->died))
		{
			pthread_mutex_lock(&(r->check_eat));
			if (time_result(p[i].last_time_eat, timestamp()) > r->time_death)
			{
				print_status(r, i, "died");
				r->died = 1;
			}
			pthread_mutex_unlock(&(r->check_eat));
			usleep(50);
		}		
		if (r->died)
			break ;
		i = 0;
		while ((r->num_limit_eat != -1) && (i < r->num_philos)
			&& (p[i].num_eat_philo >= (r->num_limit_eat - 1)))
			i++;
		if (i == r->num_philos)
			r->all_philos_ate = 1;
	}
}

void	philo_eats(t_philo *philo)
{
	t_game_rules	*rules;

	rules = philo->rules;
	pthread_mutex_lock(&(rules->forks[philo->fork_left]));
	print_status(rules, philo->id_philo, "has taken a fork");
	pthread_mutex_lock(&(rules->forks[philo->fork_right]));
	print_status(rules, philo->id_philo, "has taken a fork");
	pthread_mutex_lock(&(rules->check_eat));
	print_status(rules, philo->id_philo, "is eating");
	philo->last_time_eat = timestamp();
	pthread_mutex_unlock(&(rules->check_eat));
	eat_philo(rules->time_eat, rules);
	(philo->num_eat_philo)++;
	pthread_mutex_unlock(&(rules->forks[philo->fork_left]));
	pthread_mutex_unlock(&(rules->forks[philo->fork_right]));
}

void	*p_thread(void *philo_param)
{
	int				i;
	t_philo			*philo;
	t_game_rules	*rules;

	i = 0;
	philo = (t_philo *)philo_param;
	rules = philo->rules;
	if (philo->id_philo % 2 == 1)
		usleep(15000);
	while (!(rules->died))
	{
		philo_eats(philo);
		if (rules->all_philos_ate)
			break ;
		print_status(rules, philo->id_philo, "is sleeping");
		sleep_philo(rules->time_sleep, rules);
		print_status(rules, philo->id_philo, "is thinking");
		i++;
	}
	return (NULL);
}

int	start_dinner(t_game_rules *rules)
{
	int			i;
	t_philo		*philo;

	i = -1;
	philo = rules->philosophers;
	rules->start_time = timestamp();
	while (++i < rules->num_philos)
	{
		if (pthread_create(&(philo[i].thread_id), NULL, p_thread, &(philo[i])))
			return (1);
		philo[i].last_time_eat = timestamp();
	}
	check_death(rules, rules->philosophers);
	exit_launcher(rules, philo);
	return (0);
}
